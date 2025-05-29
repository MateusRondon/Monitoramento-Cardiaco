#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

const char* ssid = "Esp32";
const char* password = "2025mateus";

WebServer server(80);
MAX30105 sensor;

int bpm = 0;
int spo2 = 0;
unsigned long startTime = 0;
bool fingerDetected = false;
void handleBPM();
void setup() {
  Serial.begin(115200);

  // Inicializar Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());
  sensor.setup(); // configuração padrão
  sensor.setPulseAmplitudeRed(0x0A);
  sensor.setPulseAmplitudeGreen(0); // Desativa LED verde

  // Rotas
  server.on("/", handleHome);
  // server.on("/bpm", handleBPM);
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
  }

    // Aqui poderíamos usar um algoritmo mais robusto para SpO2
    spo2 = map(irValue, 50000, 100000, 90, 100); // Estimativa grosseira
  }

  Serial.printf("BPM: %d | SpO2: %d%%\n", bpm, spo2);
  delay(100);
}

void handleHome() {
  String pagina = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Monitor de Saúde</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { font-family: Arial; background-color: #ffe6f0; color: #333; text-align: center; padding: 20px; }
    canvas { max-width: 90vw; }
  </style>
</head>
<body>
  <h1>Monitor de Batimentos e Oxigenação</h1>
  <p><strong>BPM:</strong> <span id="bpm">--</span></p>
  <p><strong>SpO2:</strong> <span id="spo2">--</span>%</p>
  <canvas id="grafico" width="600" height="300"></canvas>
<script>
let ctx = document.getElementById('grafico').getContext('2d');
let labels = [];
let dataBPM = [];
let dataSpO2 = [];

let chart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: labels,
    datasets: [{
      label: 'BPM',
      borderColor: 'red',
      data: dataBPM,
      fill: false
    }, {
      label: 'SpO2',
      borderColor: 'blue',
      data: dataSpO2,
      fill: false
    }]
  },
  options: {
    animation: false,
    scales: {
      y: { beginAtZero: true, max: 150 },
      x: { display: false }
    }
  }
});

function atualizar() {
  fetch("/bpm")
    .then(r => r.json())
    .then(d => {
      document.getElementById("bpm").innerText = d.bpm;
      document.getElementById("spo2").innerText = d.oxigenacao;

      if (labels.length > 30) {
        labels.shift();
        dataBPM.shift();
        dataSpO2.shift();
      }

      labels.push('');
      dataBPM.push(d.bpm);
      dataSpO2.push(d.oxigenacao);
      chart.update();
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
  String json = "{\"bpm\":" + String(bpm) + ",\"oxigenacao\":" + String(spo2) + ",\"tempo\":" + String(tempo) + "}";
  server.send(200, "application/json", json);
}
