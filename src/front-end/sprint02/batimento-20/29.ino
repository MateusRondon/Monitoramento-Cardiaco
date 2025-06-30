#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <HTTPClient.h>

const char* ssid = "Esp32";
const char* password = "2025mateus";

const char* firebaseHost = "https://iotesp32-3725e-default-rtdb.firebaseio.com/";
const char* firebaseAuth = "n6V3fTqHztFDWs0z8CZRYWMN5TP3NE4doJhzUZHy";

WebServer server(80);
MAX30105 sensor;

int bpm = 0;
int spo2 = 0;
unsigned long startTime = 0;
bool fingerDetected = false;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000; // 5 segundos

#define STATUS_LED 2

void handleHome();
void handleBPM();
void enviarParaFirebase(int bpm, int spo2);

void setup() {
  Serial.begin(115200);
  pinMode(STATUS_LED, OUTPUT);

  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000; // 10 segundos

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFalha ao conectar ao Wi-Fi. Reiniciando...");
    ESP.restart();
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());
  digitalWrite(STATUS_LED, HIGH); // LED ligado = Wi-Fi conectado

  if (!sensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("Erro ao iniciar o sensor MAX30105. Verifique a conexão.");
    while (1); // Removível se quiser evitar travar o ESP
  }

  sensor.setup();
  sensor.setPulseAmplitudeRed(0x0A);
  sensor.setPulseAmplitudeGreen(0);

  server.on("/", handleHome);
  server.on("/bpm", handleBPM);
  server.begin();
  Serial.println("Servidor iniciado.");
  startTime = millis();
}

void loop() {
  server.handleClient();

  long irValue = sensor.getIR();

  if (irValue < 50000) {
    bpm = 0;
    spo2 = 0;
    fingerDetected = false;
    startTime = millis();
  } else {
    fingerDetected = true;
    if (checkForBeat(irValue)) {
      long beatTime = millis() - startTime;
      if (beatTime > 0) {
        bpm = 60000 / beatTime;
      }
      startTime = millis();
      spo2 = map(irValue, 50000, 100000, 90, 100);

      if (millis() - lastSendTime > sendInterval) {
        enviarParaFirebase(bpm, spo2);
        lastSendTime = millis();
      }
    }
  }

  Serial.printf("BPM: %d | SpO2: %d%%\n", bpm, spo2);
  delay(100);
}

void enviarParaFirebase(int bpm, int spo2) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(firebaseHost) + "/leituras.json?auth=" + firebaseAuth;
    String payload = "{\"bpm\":" + String(bpm) + ",\"spo2\":" + String(spo2) + ",\"timestamp\":" + String(millis()) + "}";

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.printf("Enviado para Firebase! Código: %d\n", httpResponseCode);
    } else {
      Serial.printf("Erro ao enviar: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  }
}

void handleHome() {
  String pagina = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Monitor de Saúde</title>
  <style>
    body { font-family: Arial; background-color: #ffe6f0; color: #333; text-align: center; padding: 20px; }
  </style>
</head>
<body>
  <h1>Monitor de Batimentos e Oxigenação</h1>
  <p><strong>BPM:</strong> <span id="bpm">--</span></p>
  <p><strong>SpO2:</strong> <span id="spo2">--</span>%</p>

<script>
function atualizar() {
  fetch("/bpm")
    .then(r => r.json())
    .then(d => {
      document.getElementById("bpm").innerText = d.bpm;
      document.getElementById("spo2").innerText = d.spo2;
    });
}
setInterval(atualizar, 1000);
</script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", pagina);
}

void handleBPM() {
  unsigned long tempo = fingerDetected ? (millis() - startTime) / 1000 : 0;
  String json = "{\"bpm\":" + String(bpm) + ",\"spo2\":" + String(spo2) + ",\"tempo\":" + String(tempo) + "}";
  server.send(200, "application/json", json);
}
