#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_MAX30105.h>
#include "heartRate.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// ========== CONFIGURAÇÕES ==========

// WiFi
char ssid[] = "SEU_WIFI";
char pass[] = "SUA_SENHA_WIFI";

// Blynk
char auth[] = "SEU_BLYNK_AUTH";

// BLE
BLEServer* pServer = NULL;
bool deviceConnected = false;
const char* bleName = "ESP32_Saude";

// Sensor MAX30102
Adafruit_MAX30105 particleSensor;

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Inicializando o display OLED

// Funções para enviar dados via Blynk
#define BLYNK_PRINT Serial
#define BLYNK_BPM V0
#define BLYNK_SPO2 V1

// ========== BLE CALLBACKS ==========
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Bluetooth: Dispositivo conectado.");
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Bluetooth: Dispositivo desconectado.");
    }
};

// ========== FUNÇÃO DE CONEXÃO WIFI COM TRATAMENTO DE ERRO ==========
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
  } else {
    Serial.println("\nFalha ao conectar no WiFi. Tentando novamente...");
    delay(5000); // Aguarda 5 segundos antes de tentar novamente
    connectWiFi(); // Tenta conectar novamente
  }
}

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);

  // Conexão WiFi com tratamento de erro
  connectWiFi();

  // Iniciar Blynk
  Blynk.begin(auth, ssid, pass);

  // Iniciar sensor MAX30102
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 não encontrado. Tentando novamente...");
    delay(5000); // Aguarda 5 segundos antes de tentar novamente
    if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
      Serial.println("Falha ao iniciar o MAX30102. Verifique a conexão.");
      while (1); // Parar o código se não conseguir inicializar o sensor
    }
  }

  // Configurar o sensor para leitura de oxigenação e batimentos
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A); 
  particleSensor.setPulseAmplitudeIR(0x0A);  

  // Inicializar display OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT, &Wire)) {
    Serial.println(F("Falha ao iniciar o display OLED"));
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  // Iniciar BLE
  BLEDevice::init("ESP32_Saude");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180D)); // Serviço de batimentos
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Bluetooth: Aguardando conexão...");
}

// ========== LOOP ==========
void loop() {
  Blynk.run();

  long irValue = particleSensor.getIR();

  // Verifica se o dedo foi detectado
  if (irValue > 50000) {
    float bpm = particleSensor.getHeartRate();
    int spo2 = random(95, 99); // Simulado - substitua por cálculo real se quiser

    // Verifica se a leitura do batimento foi bem-sucedida
    if (bpm > 0 && bpm < 200) {  // Limite para valores plausíveis
      Serial.print("BPM: ");
      Serial.print(bpm);
      Serial.print(" | SpO2: ");
      Serial.println(spo2);

      // Envia os dados para o app Blynk
      Blynk.virtualWrite(BLYNK_BPM, bpm);
      Blynk.virtualWrite(BLYNK_SPO2, spo2);

      // Atualiza os dados no display OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("BPM: ");
      display.print(bpm);
      display.setCursor(0, 10);
      display.print("SpO2: ");
      display.print(spo2);
      display.display();
    } else {
      Serial.println("Erro ao ler os batimentos cardíacos. Tentando novamente...");
    }

    delay(1000); // Espera 1s antes da próxima leitura
  } else {
    Serial.println("Dedo não detectado no sensor.");
    delay(1000); // Espera 1s para tentar novamente
  }

  // Verificar a conexão Bluetooth
  if (!deviceConnected) {
    Serial.println("Bluetooth: Dispositivo desconectado. Aguardando reconexão...");
    delay(5000); // Aguardar um pouco antes de tentar reconectar
  }
}
