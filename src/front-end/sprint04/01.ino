#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <PulseSensorPlayground.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

// --- Configurações do Usuário ---
// Wi-Fi
const char* ssid = "Esp32";
const char* password = "2025mateus";

// Firebase
#define FIREBASE_HOST "https://iotesp32-3725e-default-rtdb.firebaseio.com/" // Insira a URL do seu Realtime Database
#define FIREBASE_AUTH "n6V3fTqHztFDWs0z8CZRYWMN5TP3NE4doJhzUZHy"                   // Insira seu Database Secret

// --- Objetos e Variáveis Globais ---
// Web Server
WebServer server(80);

// Pulse Sensor
const int PULSE_SENSOR_PIN = 4; // Pino ADC para o Pulse Sensor
PulseSensorPlayground pulseSensor;
const int THRESHOLD = 550;

// Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String firebasePath = "/historicoUsuario"; // Caminho onde os dados serão salvos no Firebase

// Variáveis de Estado
int currentBPM = 0;
int currentSpO2 = 0; // Valor Fixo, pois o sensor não mede SpO2
unsigned long lastFirebaseUpdate = 0;
const long firebaseUpdateInterval = 5000; // Enviar dados para o Firebase a cada 5 segundos

// Perfil e Autenticação (mesmo do seu código original)
String nome = "";
String idade = "";
String email = "";
String fotoURL = "";
String usuario = "";
String senha = "";
bool logado = false;

// --- Declaração das Funções ---
void handleHome();
void handleConfig();
void handlePerfil();
void salvarPerfil();
void handleHistoricoUsuario();
void handleML();
void handleLogout();
void handleLogin();
void autenticarUsuario();
void handleRegistrar();
void registrarUsuario();
String gerarPagina(String titulo, String corpo);
void handleGetData(); // Renomeado para maior clareza
void enviarDadosParaFirebase();

// --- Setup ---
void setup() {
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configuração do Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Configuração do Pulse Sensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  pulseSensor.begin();

  // Rotas do Servidor Web
  server.on("/", handleHome);
  server.on("/config", handleConfig);
  server.on("/perfil", handlePerfil);
  server.on("/salvarPerfil", HTTP_POST, salvarPerfil);
  server.on("/historicoUsuario", handleHistoricoUsuario);
  server.on("/ml", handleML);
  server.on("/logout", handleLogout);
  server.on("/login", handleLogin);
  server.on("/autenticar", HTTP_POST, autenticarUsuario);
  server.on("/registrar", handleRegistrar);
  server.on("/registrarUsuario", HTTP_POST, registrarUsuario);
  server.on("/getData", handleGetData); // Rota para os gráficos
  server.begin();
}

// --- Loop Principal ---
void loop() {
  server.handleClient();

  if (pulseSensor.sawNewSample()) {
    if (pulseSensor.sawStartOfBeat()) {
      currentBPM = pulseSensor.getBeatsPerMinute();
      // Valor de SpO2 é fixo (placeholder)
      if (currentBPM > 40) { // Filtro simples para leituras válidas
          currentSpO2 = 98;
      }
    }
  }

  // Lógica para detectar ausência do dedo
  if (pulseSensor.getLatestSample() < (THRESHOLD / 2)) {
      currentBPM = 0;
      currentSpO2 = 0;
  }
  
  // Envia os dados para o Firebase em intervalos definidos
  if (millis() - lastFirebaseUpdate > firebaseUpdateInterval && currentBPM > 0) {
    enviarDadosParaFirebase();
    lastFirebaseUpdate = millis();
  }

  delay(20);
}

// --- Funções do Servidor Web ---

String gerarPagina(String titulo, String corpo) {
  // A função gerarPagina foi mantida, mas com a adição da biblioteca Chart.js
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + titulo + "</title>"
         // Inclui a biblioteca Chart.js
         "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>"
         "<style>"
         "body{font-family:Arial, sans-serif;margin:0;padding:0;display:flex;background-color:#f4f4f4;}"
         ".menu{position:fixed;top:0;left:0;width:220px;height:100vh;background:#e91e63;color:white;padding-top:20px;display:flex;flex-direction:column;align-items:start;gap:18px;padding-left:20px;z-index:10;box-shadow:2px 0 5px rgba(0,0,0,0.1);}"
         ".menu a{color:white;text-decoration:none;font-weight:bold;display:flex;align-items:center;gap:10px;font-size:17px;transition:background 0.2s;width:100%;padding:5px 0;}"
         ".menu a:hover{background:rgba(255,255,255,0.1);}"
         ".content{margin-left:220px;padding:25px;width:calc(100% - 220px);}"
         ".card{background:white;padding:25px;border-radius:15px;margin:20px 0;box-shadow:0 4px 12px rgba(0,0,0,0.1);}"
         ".data-display{display:flex;justify-content:space-around;text-align:center;}"
         ".data-item h2{font-size:2.5em;color:#e91e63;margin:0;}"
         ".data-item p{color:#666;margin:0;}"
         "h1{color:#333;}"
         "button,input[type='submit']{padding:12px 25px;font-size:16px;margin:10px 5px;border:none;border-radius:25px;background-color:#e91e63;color:white;cursor:pointer;box-shadow:0 4px 8px rgba(0,0,0,0.2);transition:background-color 0.3s ease;}"
         "button:hover,input[type='submit']:hover{background-color:#d81b60;}"
         "input[type='text'],input[type='password']{padding:12px;border-radius:10px;border:1px solid #ccc;width:calc(100% - 24px); margin-bottom: 10px;}"
         "</style>"
         "</head><body>"
         "<div class='menu'>"
         "<a href='/'>&#127968; Home</a>"
         "<a href='/historicoUsuario'>&#128196; Histórico</a>"
         "<a href='/perfil'>&#128100; Perfil</a>"
         "<a href='/ml'>&#129302; ML</a>"
         "<a href='/logout'>&#128682; Logout</a>"
         "</div>"
         "<div class='content'><h1>" + titulo + "</h1>" + corpo + "</div></body></html>";
}

void handleHome() {
  String corpo =
    "<div class='card data-display'>"
    "  <div class='data-item'>"
    "    <h2 id='bpmValue'>--</h2><p>❤️ BPM</p>"
    "  </div>"
    "  <div class='data-item'>"
    "    <h2 id='spo2Value'>--</h2><p>💨 SpO2 (%)</p>"
    "  </div>"
    "</div>"
    "<div class='card'>"
    "  <h2>Gráfico em Tempo Real</h2>"
    "  <canvas id='healthChart'></canvas>" // Único canvas para os dois gráficos
    "</div>"
    "<script>"
    "  const bpmValue = document.getElementById('bpmValue');"
    "  const spo2Value = document.getElementById('spo2Value');"
    "  const ctx = document.getElementById('healthChart').getContext('2d');"
    "  let healthChart = new Chart(ctx, {"
    "    type: 'line',"
    "    data: {"
    "      labels: [],"
    "      datasets: ["
    "        { label: 'BPM', data: [], borderColor: 'rgba(233, 30, 99, 1)', backgroundColor: 'rgba(233, 30, 99, 0.2)', tension: 0.3, fill: true },"
    "        { label: 'SpO2', data: [], borderColor: 'rgba(76, 175, 80, 1)', backgroundColor: 'rgba(76, 175, 80, 0.2)', tension: 0.3, fill: true }"
    "      ]"
    "    },"
    "    options: { animation: { duration: 500 }, scales: { y: { beginAtZero: true } } }"
    "  });"
    "  function updateData() {"
    "    fetch('/getData')"
    "      .then(response => response.json())"
    "      .then(data => {"
    "        bpmValue.textContent = data.bpm > 0 ? data.bpm : '--';"
    "        spo2Value.textContent = data.spo2 > 0 ? data.spo2 : '--';"
    "        if(data.bpm > 0) {"
    "          const chart = healthChart.data;"
    "          if(chart.labels.length > 20) {" // Manter no máximo 20 pontos
    "            chart.labels.shift();"
    "            chart.datasets.forEach((dataset) => { dataset.data.shift(); });"
    "          }"
    "          chart.labels.push(new Date().toLocaleTimeString());"
    "          chart.datasets[0].data.push(data.bpm);"
    "          chart.datasets[1].data.push(data.spo2);"
    "          healthChart.update('none');" // 'none' para uma animação mais suave
    "        }"
    "      });"
    "  }"
    "  setInterval(updateData, 2000);" // Atualiza a cada 2 segundos
    "</script>";

  server.send(200, "text/html", gerarPagina("Monitor de Saúde", corpo));
}

void handleGetData() {
  String json = "{";
  json += "\"bpm\": " + String(currentBPM) + ",";
  json += "\"spo2\": " + String(currentSpO2);
  json += "}";
  server.send(200, "application/json", json);
}

void enviarDadosParaFirebase() {
  if (WiFi.status() == WL_CONNECTED && currentBPM > 0) {
    // Usamos um objeto JSON para enviar os dados
    FirebaseJson json;
    json.set("bpm", String(currentBPM));
    json.set("spo2", String(currentSpO2));
    //json.set("timestamp", ".sv", "timestamp"); // Carimbo de data/hora do servidor Firebase

    String nodePath = firebasePath + "/" + String(usuario); // Salva sob o nome do usuário logado
    
    // Usamos push para criar um ID único para cada entrada
    // if (Firebase.pushJSON(fbdo, nodePath, json)) {
    //   Serial.println("Dados enviados para o Firebase com sucesso.");
    //   Serial.println(fbdo.pushName()); // Mostra o ID único gerado
    // } else {
    //   Serial.println("ERRO ao enviar para o Firebase:");
    //   Serial.println(fbdo.errorReason());
    // }
  }
}


// --- Funções de Perfil, Login, etc. ---
// (O restante das funções como handlePerfil, handleLogin, etc., são mantidas exatamente como no seu código original, pois já são funcionais.)
void handlePerfil() {
 if (!logado) {
  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "");
  return;
 }
 String corpo = "<div class='card'><form action='/salvarPerfil' method='POST'>"
                "<p><label>Nome: <input name='nome' value='" + nome + "'></label></p>"
                "<p><label>Idade: <input name='idade' value='" + idade + "'></label></p>"
                "<p><label>Email: <input name='email' value='" + email + "'></label></p>"
                "<p><label>Foto URL: <input name='foto' value='" + fotoURL + "'></label></p>"
                "<p><label>Login: <input name='login' value='" + usuario + "'></label></p>"
                "<p><label>Senha: <input type='password' name='senha' value='" + senha + "'></label></p>"
                "<input type='submit' value='Salvar Perfil'>"
                "</form></div>";
 server.send(200, "text/html", gerarPagina("Perfil do Usuário", corpo));
}

void salvarPerfil() {
 nome = server.arg("nome");
 idade = server.arg("idade");
 email = server.arg("email");
 fotoURL = server.arg("foto");
 usuario = server.arg("login");
 senha = server.arg("senha");
 String corpo = "<div class='card'><p>Perfil salvo com sucesso!</p><a href='/perfil'><button>Voltar</button></a></div>";
 server.send(200, "text/html", gerarPagina("Perfil Salvo", corpo));
}

void handleHistoricoUsuario() {
 if (!logado) {
  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "");
  return;
 }
 // Esta página agora pode ser usada para visualizar dados do Firebase no futuro.
 String corpo = "<div class='card'><h2>Histórico do Usuário</h2>"
                "<p><strong>Nome:</strong> " + nome + "</p>"
                "<p><strong>Idade:</strong> " + idade + "</p>"
                "<p><strong>Email:</strong> " + email + "</p>"
                "<p><img src='" + fotoURL + "' alt='Foto' width='100'></p></div>"
                "<div class='card'><p>A visualização do histórico do Firebase pode ser implementada aqui.</p></div>";
 server.send(200, "text/html", gerarPagina("Histórico do Perfil", corpo));
}

void handleML() {
 String corpo = "<div class='card'><h2>Machine Learning</h2><p>Acesse nosso modelo de Machine Learning no Google Colab:</p><a href='https://colab.research.google.com/drive/1PAZt0PLthIQHCQyrR3LrJpZ7q_Sgp8_0?usp=sharing' target='_blank'><button>Abrir Colab</button></a></div>";
 server.send(200, "text/html", gerarPagina("ML", corpo));
}

void handleLogout() {
 logado = false;
 server.send(200, "text/html", gerarPagina("Logout", "<div class='card'><p>Logout realizado com sucesso.</p><a href='/login'><button>Login</button></a></div>"));
}

void handleLogin() {
 String corpo = "<div class='card' style='max-width:400px;text-align:center;'>"
                "<h2 style='margin-bottom:20px;'>Login</h2>"
                "<form method='POST' action='/autenticar'>"
                "<p><input name='usuario' placeholder='Usuário'></p>"
                "<p><input type='password' name='senha' placeholder='Senha'></p>"
                "<input type='submit' value='Entrar'>"
                "</form>"
                "<p style='margin-top:20px;'>Não tem uma conta?</p>"
                "<a href='/registrar'><button>Registrar</button></a>"
                "</div>";
 server.send(200, "text/html", gerarPagina("Login", corpo));
}

void autenticarUsuario() {
 String u = server.arg("usuario");
 String s = server.arg("senha");
 if (u == usuario && s == senha && u != "") {
  logado = true;
  server.sendHeader("Location", "/perfil");
  server.send(302, "text/plain", "");
 } else {
  server.send(200, "text/html", gerarPagina("Erro de Login", "<div class='card'><p>Login inválido.</p><a href='/login'><button>Tentar Novamente</button></a></div>"));
 }
}

void handleRegistrar() {
 String corpo = "<div class='card' style='max-width:400px;text-align:center;'>"
                "<h2 style='margin-bottom:20px;'>Registrar</h2>"
                "<form method='POST' action='/registrarUsuario'>"
                "<p><input name='usuario' placeholder='Usuário'></p>"
                "<p><input type='password' name='senha' placeholder='Senha'></p>"
                "<input type='submit' value='Registrar'>"
                "</form>"
                "<p style='margin-top:20px;'>Já tem uma conta?</p>"
                "<a href='/login'><button>Login</button></a>"
                "</div>";
 server.send(200, "text/html", gerarPagina("Registro de Usuário", corpo));
}

void registrarUsuario() {
 usuario = server.arg("usuario");
 senha = server.arg("senha");
 logado = true;
 server.send(200, "text/html", gerarPagina("Usuário Registrado", "<div class='card'><p>Usuário registrado com sucesso!</p><a href='/perfil'><button>Ir para Perfil</button></a></div>"));
}

void handleConfig() {
 // Função mantida por compatibilidade
 String corpo = "<div class='card'><h2>Configurações</h2><p>Configurações gerais do sistema.</p></div>";
 server.send(200, "text/html", gerarPagina("Configuração", corpo));
}