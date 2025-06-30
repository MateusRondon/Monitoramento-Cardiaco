#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include "MAX30105.h"   // Biblioteca MAX3010x
#include "heartRate.h"  // Algoritmo de cálculo da frequência cardíaca

MAX30105 particleSensor;  

const byte RATE_SIZE = 4;  // Aumente este valor para amostras maiores. 4 está bom.
byte rates[RATE_SIZE];     // Array da frequência cardíaca
byte rateSpot = 0;
long lastBeat = 0;  // Tempo que o último batimento ocorreu
float beatsPerMinute;
int beatAvg;
const char* ssid = "Slimelife";
const char* password = "12345678";
WebServer server(80);  // Servidor na porta 80
void setup() {
  Serial.begin(115200);
  //  Inicia o Sensor
  particleSensor.begin(Wire, I2C_SPEED_FAST);  // Usa as portas I2C padrão, 400kHz de velocidade
  particleSensor.setup();                      // Configura o sensor para as configurações padrões
  particleSensor.setPulseAmplitudeRed(0x0A);   // Liga o LED vermelho para indicar se o sensor está funcionando.

  Wire.begin();
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  long irValue = particleSensor.getIR();  // Lê o sensor infravermelho (IR) vai permitir sabermos se o dedo está no sensor ou não
                                          // E também detectar um batimento
  if (irValue > 7000) {                   // Caso seja detectado o dedo
    Serial.print(beatAvg);
    Serial.println(" BPM");

    if (checkForBeat(irValue) == true)  // Se um batimento for detectado
    {
      Serial.print(beatAvg);
      Serial.println(" BPM");

      // Nós sentimos um batimento!
      long delta = millis() - lastBeat;  // Calcula a duração entre dois batimentos
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);  // Calculando o BPM

      if (beatsPerMinute < 255 && beatsPerMinute > 20)  // Para calcular a média nós salvamos alguns valores (4) então tiramos a média
      {
        rates[rateSpot++] = (byte)beatsPerMinute;  // Salva esta leitura no array
        rateSpot %= RATE_SIZE;

        // Pega o valor da média
        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
  }
  if (irValue < 7000) {  // Se não for detectado o dedo, informe ao usuário
    beatAvg = 0;
    Serial.println("Por favor, coloque seu dedo.");
  }
}
