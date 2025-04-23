#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

// Configurações de WiFi
const char* ssid = "Slimelife";
const char* password = "12345678";

WebServer server(80);  // Servidor na porta 80

MAX30105 particleSensor;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;
bool fingerDetected = false;


void handleRoot() {
  String html = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Monitor Cardiaco</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; background-color: #f5f5f5; }
      .container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
      h1 { color: #333; }
      .bpm-display { font-size: 3em; margin: 20px 0; color: #e74c3c; font-weight: bold; }
      .status { font-size: 1.2em; margin-bottom: 20px; padding: 10px; border-radius: 5px; }
      .finger-detected { background-color: #d4edda; color: #155724; }
      .finger-not-detected { background-color: #f8d7da; color: #721c24; }
      .chart-container { width: 100%; height: 300px; margin-top: 30px; }
      .info { margin-top: 20px; font-size: 0.9em; color: #666; }
    </style>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  </head>
  <body>
    <div class="container">
      <h1>Monitor Cardíaco com MAX30102</h1>
      <div id="status" class="status finger-not-detected">Por favor, coloque seu dedo no sensor</div>
      <div class="bpm-display" id="bpm">--</div> 
      <div>Batimentos por minuto (BPM)</div>
      
      <div class="chart-container">
        <canvas id="bpmChart"></canvas>
      </div>
      
      <div class="info">
        <p>Conectado ao IP: )====";
  html += WiFi.localIP().toString();
  html += R"====(</p>
        <p>Atualizado em: <span id="updateTime"></span></p>
      </div>
    </div>
    
    <script>
      const bpmElement = document.getElementById('bpm');
      const statusElement = document.getElementById('status');
      const updateTimeElement = document.getElementById('updateTime');
      const ctx = document.getElementById('bpmChart').getContext('2d');
      
      let bpmChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: [],
          datasets: [{
            label: 'BPM',
            data: [],
            borderColor: 'rgba(231, 76, 60, 1)',
            borderWidth: 2,
            tension: 0.1,
            fill: false,
            pointBackgroundColor: 'rgba(231, 76, 60, 1)'
          }]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          scales: {
            y: {
              beginAtZero: false,
              suggestedMin: 50,
              suggestedMax: 120,
              grid: {
                color: 'rgba(0, 0, 0, 0.1)'
              }
            },
            x: {
              grid: {
                display: false
              }
            }
          },
          plugins: {
            legend: {
              display: false
            }
          }
        }
      });
      
      let timeCounter = 0;
      const maxDataPoints = 20;
      
      function updateData() {
        fetch('/data')
          .then(response => response.json())
          .then(data => {
            const now = new Date();
            updateTimeElement.textContent = now.toLocaleTimeString();
            
            if(data.fingerDetected) {
              bpmElement.textContent = data.bpm;
              statusElement.textContent = "Dedo detectado - medindo...";
              statusElement.className = "status finger-detected";
              
              // Atualiza o gráfico
              if(bpmChart.data.labels.length >= maxDataPoints) {
                bpmChart.data.labels.shift();
                bpmChart.data.datasets[0].data.shift();
              }
              
              bpmChart.data.labels.push(timeCounter++);
              bpmChart.data.datasets[0].data.push(data.bpm);
              bpmChart.update();
            } else {
              bpmElement.textContent = "--";
              statusElement.textContent = "Por favor, coloque seu dedo no sensor";
              statusElement.className = "status finger-not-detected";
            }
          })
          .catch(error => console.error('Erro:', error));
          
        setTimeout(updateData, 1000);
      }
      
      updateData();
    </script>
  </body>
  </html>
  )=====";
  
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{\"bpm\":" + String(beatAvg) + 
                ",\"fingerDetected\":" + String(fingerDetected ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}
void setup() {
  Serial.begin(115200);
   // Configuração do sensor
  byte ledBrightness = 0x1F; // 0=desligado a 0xFF=mais brilhante
  byte sampleAverage = 4; // Média de 4 amostras
  byte ledMode = 2; // Usar LED vermelho e IR
  int sampleRate = 100; // 100 amostras por segundo
  int pulseWidth = 411; // 411 microsegundos
  int adcRange = 4096; // Faixa de 4096 nA
  // Inicializa o sensor

  particleSensor.begin(Wire,I2C_SPEED_FAST);
  Wire.begin();
  long irValue = particleSensor.getIR();
  if (checkForBeat(irValue) == false) {
    Serial.println("MAX30102 não foi encontrado. Por favor, verifique a conexão.");
    while (1);
  }
  // particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  // Conecta ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! IP: s  ");
  Serial.println(WiFi.localIP());
  
  // Configura as rotas do servidor web
  server.on("/", handleRoot);
   server.on("/data", handleData);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
  
  long irValue =0;
  
  if (irValue > 7000) {
    fingerDetected = true;
    
    if (checkForBeat(irValue) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20) {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;

        beatAvg = 0;
        for (byte x = 0; x < RATE_SIZE; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
  } else {
    fingerDetected = false;
    beatAvg = 0;
  }
  
  delay(10); // Pequeno delay para estabilidade
}


