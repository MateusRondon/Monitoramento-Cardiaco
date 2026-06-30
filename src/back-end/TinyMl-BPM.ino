/*
 * ================================================================
 * CoreAI ESP32 — Monitor Cardíaco (Otimizado - Versão Leve)
 * ================================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <ArduinoOTA.h>
#include <FirebaseESP32.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// ================================================================
// CONFIGURAÇÕES GERAIS (Reduzidas para economizar memória)
// ================================================================
#define USE_TINYML_MODEL   false
#define USE_BLUETOOTH      false // Mantido em FALSE para caber na memória padrão!
#define USE_LEAD_OFF_DETECT false

// ===== CONFIG REDE =====
const char* ssid     = "User-Mateus";
const char* password = "2025mateusr@";

// ===== CONFIG FIREBASE =====
#define FIREBASE_HOST "monitor-cardiaco-mateus-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "f7xMp57xEciA0prkc0vNUNp3jyTKmgMWeb5Ich2D"

// ===== CONFIG GEMINI (IA) =====
const String apiKey  = "AIzaSyBijqirx8e3G3vgS4rKo3OOPt0anOnlCVw";
const String endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=";

// ===== PINAGEM =====
const int PIN_OUTPUT   = 34;   // AD8232 OUT
const int BTN_UP       = 26;   // Menu para cima
const int BTN_DOWN     = 27;   // Menu para baixo
const int BTN_CONFIRM  = 14;   // Ligar sistema de coleta

// ===== OLED =====
#define OLED_W      128
#define OLED_H      64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 oled(OLED_W, OLED_H, &Wire, -1);
bool oledOk = false;

// ================================================================
// TIPOS E ESTRUTURAS
// ================================================================
struct HrvFeatures { float hrBpm; float rmssd; float sdnn; float pnn50; };

struct TinyMLResult {
  String label; float confidence;
  float rmssd; float sdnn; float pnn50;
};

enum MenuScreen { SCREEN_STATUS, SCREEN_BPM, SCREEN_TINYML, SCREEN_IA, SCREEN_COUNT };

// ===== OBJETOS GLOBAIS =====
FirebaseData   fbdo;
FirebaseAuth   auth;
FirebaseConfig config;
WebServer      server(80);

// ===== VARIÁVEIS DO SISTEMA =====
volatile float        BPM             = 0;
volatile int          ECG_RAW         = 0;
unsigned long         ultimoEnvioFirebase = 0;
uint32_t              tsLastReport    = 0;
unsigned long         ultimaAnaliseIA = 0;
unsigned long         ultimoPico      = 0;
float                 mediaBPM        = 0;
String                ultimoParecerIA = "Aguardando analise...";

volatile bool         systemOn        = false; // Inicia desligado (GPIO 14 ativa)

// ===== DETECÇÃO HRV =====
volatile float        ecgBaseline     = 2048.0f;
volatile float        ecgEnvelope     = 120.0f;
volatile bool         beatArmed       = true;

#define RR_BUFFER_SIZE 60 // Reduzido para economizar RAM
uint16_t       rrIntervals[RR_BUFFER_SIZE];
uint8_t        rrCount    = 0;
unsigned long  lastPeakMs = 0;

TinyMLResult tinymlResult = { "Aguardando", 0.0f, 0, 0, 0 };
int currentScreen = SCREEN_STATUS;

// ================================================================
// ANÁLISE COM GEMINI (IA)
// ================================================================
void analisarComGemini() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  Serial.println("🧠 Consultando Gemini...");
  HTTPClient http;
  http.begin(endpoint + apiKey);
  http.addHeader("Content-Type", "application/json");

  // Prompt simplificado para economizar memória dinâmica
  String prompt = "Paciente com " + String((int)BPM) + " BPM. Modelo detectou '" + tinymlResult.label + "'. Diga em 1 frase curta se é estavel ou risco.";
  String payload = "{\"contents\":[{\"parts\":[{\"text\":\"" + prompt + "\"}]}]}";
  
  int code = http.POST(payload);
  if (code == 200) {
    DynamicJsonDocument doc(1024); // Buffer reduzido
    deserializeJson(doc, http.getString());
    ultimoParecerIA = doc["candidates"][0]["content"]["parts"][0]["text"].as<String>();
    Firebase.setString(fbdo, "/analise_ia/ultimo_parecer", ultimoParecerIA);
    Serial.println("🤖 IA: " + ultimoParecerIA);
  }
  http.end();
}

// ================================================================
// CÁLCULO HRV (Simplificado)
// ================================================================
HrvFeatures computeHRV() {
  HrvFeatures f = { 0, 0, 0, 0 };
  if (rrCount < 4) return f;
  uint8_t n = rrCount;

  float meanRR = 0, sdnnSum = 0, rmssdSum = 0; uint8_t nn50 = 0;
  for (uint8_t i = 0; i < n; i++) meanRR += rrIntervals[i];
  meanRR /= n;
  f.hrBpm = 60000.0f / meanRR;

  for (uint8_t i = 0; i < n; i++) sdnnSum += pow(rrIntervals[i] - meanRR, 2);
  f.sdnn = sqrt(sdnnSum / n);

  for (uint8_t i = 1; i < n; i++) {
    float diff = abs((int16_t)rrIntervals[i] - (int16_t)rrIntervals[i - 1]);
    rmssdSum += diff * diff;
    if (diff > 50.0f) nn50++;
  }
  f.rmssd = sqrt(rmssdSum / (n - 1));
  f.pnn50 = (float)nn50 / (float)(n - 1) * 100.0f;
  return f;
}

// ================================================================
// CLASSIFICAÇÃO BÁSICA (Sem modelo pesado)
// ================================================================
TinyMLResult runTinyML() {
  TinyMLResult r;
  HrvFeatures hrv = computeHRV();
  r.rmssd = hrv.rmssd; r.sdnn = hrv.sdnn; r.pnn50 = hrv.pnn50;

  if (rrCount < 4) { r.label = "Aguardando"; r.confidence = 0.0f; return r; }

  float hr = (BPM > 0) ? BPM : hrv.hrBpm;
  if (hr < 40)        { r.label = "Bradicardia"; r.confidence = 0.95f; }
  else if (hr > 120)  { r.label = "Taquicardia"; r.confidence = 0.95f; }
  else if (hrv.sdnn > 100.0f) { r.label = "Arritmia"; r.confidence = 0.80f; }
  else { r.label = "Normal"; r.confidence = 0.90f; }
  
  return r;
}

// ================================================================
// ENVIO PARA O FIREBASE (Otimizado)
// ================================================================
void enviarDadosEAlertas() {
  if (!Firebase.ready()) return;
  
  FirebaseJson json;
  json.add("bpm_ajustado", (int)BPM);
  json.add("valor_bruto", ECG_RAW);
  json.add("estado_sistema", systemOn);
  json.add("tinyml_class", tinymlResult.label);
  
  Firebase.setJSON(fbdo, "/status_atual", json);
  
  // Limpa buffer para economizar RAM
  json.clear();
  json.add("hr", (int)BPM);
  json.add("rmssd", tinymlResult.rmssd);
  json.add("sdnn", tinymlResult.sdnn);
  Firebase.setJSON(fbdo, "/status_atual/tinyml_features", json);

  Serial.println("🚀 [FIREBASE] Sincronizado!");
}

// ================================================================
// INTERFACE WEB LOCAL
// ================================================================
void handleRoot() {
  String h = "<html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='3'>";
  h += "<style>body{background:#0f172a;color:white;font-family:sans-serif;text-align:center;padding:20px;}</style></head><body>";
  h += "<h1>Monitor Mateus</h1>";
  h += "<h3>Sistema: " + String(systemOn ? "LIGADO" : "DESLIGADO") + "</h3>";
  h += "<h2>BPM: " + String((int)BPM) + "</h2>";
  h += "<h2>RAW: " + String(ECG_RAW) + "</h2>";
  h += "<h3>IA: " + tinymlResult.label + "</h3>";
  h += "</body></html>";
  server.send(200, "text/html", h);
}

// ================================================================
// DISPLAY OLED
// ================================================================
void updateOLED() {
  if (!oledOk) return;
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  
  oled.setCursor(0, 0); oled.print(systemOn ? "ON " : "OFF");
  oled.setCursor(40, 0); oled.print(WiFi.status() == WL_CONNECTED ? "WiFi" : "Sem Rede");
  oled.setCursor(100, 0); oled.print((int)BPM);
  oled.drawLine(0, 11, 127, 11, SSD1306_WHITE);

  if (!systemOn) {
    oled.setCursor(0, 30); oled.print("SISTEMA DESLIGADO");
    oled.setCursor(0, 50); oled.print("Aperte GPIO 14 p/ ligar");
  } else {
    oled.setCursor(0, 20); oled.print("Modo: Status Atual");
    oled.setCursor(0, 35); oled.print("BPM: " + String((int)BPM));
    oled.setCursor(0, 50); oled.print("IA: " + tinymlResult.label);
  }
  oled.display();
}

// ================================================================
// TASK FREERTOS - Coleta de Dados do AD8232
// ================================================================
void taskECG(void* p) {
  for (;;) {
    unsigned long agora = millis();

    if (!systemOn) {
      vTaskDelay(100 / portTICK_PERIOD_MS);
      continue;
    }

    int raw = analogRead(PIN_OUTPUT);
    ECG_RAW = raw;

    ecgBaseline = ecgBaseline * 0.99f + raw * 0.01f;
    float dev = raw - ecgBaseline;
    if (dev > ecgEnvelope) ecgEnvelope = dev;
    else ecgEnvelope *= 0.995f;
    
    if (ecgEnvelope < 60.0f) ecgEnvelope = 60.0f;
    float thr = ecgEnvelope * 0.6f;

    if (beatArmed && dev > thr) {
      beatArmed = false;
      if (agora - ultimoPico > 250) {
        float instBpm = 60000.0f / (agora - ultimoPico);
        if (instBpm >= 40 && instBpm <= 200) {
          mediaBPM = (mediaBPM * 0.7f) + (instBpm * 0.3f);
          BPM = mediaBPM;
          uint16_t rr = agora - lastPeakMs;
          if (rr > 250 && rr < 2000) rrIntervals[rrCount++ % RR_BUFFER_SIZE] = rr;
          lastPeakMs = agora;
        }
        ultimoPico = agora;
      }
    }
    if (dev < thr * 0.5f) beatArmed = true;

    if (agora - ultimoPico > 2500) { BPM = 0; mediaBPM = 0; rrCount = 0; }
    
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

// ================================================================
// SETUP
// ================================================================
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Evita reset por queda de tensão
  Serial.begin(115200);
  
  pinMode(BTN_UP,      INPUT_PULLUP);
  pinMode(BTN_DOWN,    INPUT_PULLUP);
  pinMode(BTN_CONFIRM, INPUT_PULLUP); // Botão principal no GPIO 14

  Wire.begin(21, 22);
  if (oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { oledOk = true; }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && millis() < 10000) { delay(500); }

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  fbdo.setBSSLBufferSize(1024, 512); // Reduzido drasticamente para caber na RAM
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  xTaskCreatePinnedToCore(taskECG, "TaskECG", 2048, NULL, 1, NULL, 0);

  server.on("/", handleRoot);
  server.begin();
  ArduinoOTA.begin();

  Serial.println("✅ Sistema Pronto. Aperte GPIO 14 para iniciar.");
}

// ================================================================
// LOOP PRINCIPAL
// ================================================================
void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  // Leitura simples do botão
  static unsigned long ultBtn = 0;
  if (digitalRead(BTN_CONFIRM) == LOW && millis() - ultBtn > 300) {
    systemOn = !systemOn;
    ultBtn = millis();
    if (!systemOn) { BPM = 0; ECG_RAW = 0; }
    Serial.println(systemOn ? "🟢 Coleta LIGADA" : "🔴 Coleta DESLIGADA");
  }

  static uint32_t lastOled = 0;
  if (millis() - lastOled > 200) { updateOLED(); lastOled = millis(); }

  if (systemOn && millis() - tsLastReport > 1000) {
    tinymlResult = runTinyML();
    Serial.printf("💓 BPM:%.0f | RAW:%d | IA:%s\n", BPM, ECG_RAW, tinymlResult.label.c_str());
    tsLastReport = millis();
  }

  if (systemOn && millis() - ultimoEnvioFirebase > 5000) {
    enviarDadosEAlertas();
    ultimoEnvioFirebase = millis();
  }

  if (systemOn && millis() - ultimaAnaliseIA > 120000 && BPM > 0) {
    analisarComGemini();
    ultimaAnaliseIA = millis();
  }
}