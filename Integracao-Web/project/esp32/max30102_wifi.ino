#include <Wire.h>
#include "MAX30105.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Your server endpoint
const char* serverUrl = "http://your-server/api/vitals";

// Initialize sensor
MAX30105 particleSensor;

// Variables for calculations
const byte RATE_SIZE = 4; // Increase this for more averaging
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;
float spO2;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 not found!");
    while (1);
  }

  // Configure sensor settings
  particleSensor.setup(
    60,    // LED brightness (0-255)
    4,     // Sample averaging
    2,     // LED mode (2 = Red + IR)
    400,   // Sample rate (Hz)
    411,   // Pulse width (μs)
    4096   // ADC range
  );

  // Set up the IR LED for proximity/SpO2 detection
  particleSensor.enableDIETEMPRDY();
}

void loop() {
  long irValue = particleSensor.getIR();
  long redValue = particleSensor.getRed();

  // Check if finger is detected
  if (irValue > 50000) {
    // Calculate heart rate
    if (checkForBeat(irValue)) {
      long delta = millis() - lastBeat;
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;

        // Calculate average heart rate
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }

    // Calculate SpO2
    float ratio = (float)redValue / (float)irValue;
    spO2 = 110.0 - 25.0 * ratio; // Simplified SpO2 calculation

    // Ensure SpO2 is within valid range
    if (spO2 > 100) spO2 = 100;
    if (spO2 < 80) spO2 = 80;

    // Send data to server if WiFi is connected
    if (WiFi.status() == WL_CONNECTED) {
      sendData(beatAvg, spO2);
    }

    // Debug output
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);
    Serial.print(", SpO2=");
    Serial.println(spO2);
  } else {
    Serial.println("No finger detected");
  }

  delay(100); // Small delay between readings
}

void sendData(int heartRate, float spO2) {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Create JSON document
  StaticJsonDocument<200> doc;
  doc["heart_rate"] = heartRate;
  doc["spo2"] = spO2;
  doc["device_id"] = "ESP32_001";

  String jsonString;
  serializeJson(doc, jsonString);

  // Send POST request
  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  } else {
    Serial.printf("Error code: %d\n", httpResponseCode);
  }

  http.end();
}