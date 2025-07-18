#include <Wire.h>
#include <WiFi.h>
#include <MAX30105.h>
#include <ESPAsyncWebServer.h>

// ========== CONFIGURAÇÕES ==========

// WiFi
const char* ssid = "Cba-if";    // Substitua pelo seu SSID
const char* pass = "12345678";     // Substitua pela sua senha

// Criação do objeto MAX30105
MAX30105 particleSensor;
long lastBeat = 0;
float bpm = 0;
int rates[4]; // Média móvel de 4 valores
byte rateSpot = 0;

// Variáveis de SpO2 simuladas (já que o sensor MAX30102 não tem cálculo direto aqui)
int simulatedSpo2 = 98;  // Simulando o valor de SpO2

// Criação do servidor Web
AsyncWebServer server(80);

// ========== CONEXÃO WiFi ==========

void connectWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, pass);
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar no WiFi. Tentando novamente...");
    delay(5000);
    connectWiFi();
  }
}

// ========== SETUP ==========

void setup() {
  Serial.begin(115200);
  connectWiFi();
  
  // Iniciar o sensor MAX30102
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 não encontrado. Verifique a conexão.");
    while (1);
  }

  // Configurações típicas para batimentos
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); // Pouca luz
 // particleSensor.setPulseAmplitudeIR(0x0A);

  // Rota principal para exibir os dados do sensor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>Dados do Sensor MAX30102</h1>";
    html += "<p>Frequência Cardíaca (bpm): " + String(bpm) + "</p>";
    html += "<p>SpO2: " + String(simulatedSpo2) + "</p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Iniciar o servidor
  server.begin();
}

// ========== LOOP ==========

void loop() {
  long irValue = particleSensor.getIR();

  if (irValue > 50000) { // Dedo detectado
    // Calcula o tempo entre os batimentos
    long delta = millis() - lastBeat;
    lastBeat = millis();

    // Calcula a frequência cardíaca
    if (delta > 0) {
      float beatsPerMinute = 60 / (delta / 1000.0);
      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        rates[rateSpot++] = (int)beatsPerMinute;
        rateSpot %= 4;

        bpm = 0;
        for (byte x = 0; x < 4; x++)
          bpm += rates[x];
        bpm /= 4;

        // Simulando o valor de SpO2
        simulatedSpo2 = random(95, 99);

        // Exibir no Serial Monitor
        Serial.print("BPM: ");
        Serial.print(bpm);
        Serial.print(" | SpO2: ");
        Serial.println(simulatedSpo2);
      }
    }
  } else {
    Serial.println("Dedo não detectado no sensor.");
  }

  delay(100); // Taxa de amostragem razoável
}
