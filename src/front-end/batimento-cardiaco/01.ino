#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <PulseSensorPlayground.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

/*
   Código Refinado e Simplificado
   - A funcionalidade de gráfico foi removida para reduzir o tamanho do código.
   - A página Home agora exibe apenas os valores numéricos de BPM e SpO2.
   - A integração com o Firebase para salvar os dados está ativa.
*/

// --- Configurações do Usuário ---
// Wi-Fi
const char* ssid = "Esp32";
const char* password = "2025mateus";

// Firebase
#define FIREBASE_HOST "https://iotesp32-3725e-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "n6V3fTqHztFDWs0z8CZRYWMN5TP3NE4doJhzUZHy"

// --- Objetos e Variáveis Globais ---
WebServer server(80);

// Pulse Sensor
const int PULSE_SENSOR_PIN = 4;
PulseSensorPlayground pulseSensor;
const int THRESHOLD = 550;

// Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String firebaseBasePath = "/leituras"; // Caminho base para salvar os dados

// Variáveis de Estado
int currentBPM = 0;
int currentSpO2 = 0; // Lembrete: Valor fixo, pois o sensor não mede SpO2
unsigned long lastFirebaseUpdate = 0;
const long FIREBASE_UPDATE_INTERVAL = 5000; // Enviar dados a cada 5 segundos

// Perfil e Autenticação
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
void handleGetData();
void enviarDadosParaFirebase();

// --- Setup ---
void setup() {
  Serial.begin(115200);

  // Conexão Wi-Fi
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
  server.on("/getData", handleGetData);
  server.begin();
}

// --- Loop Principal ---
void loop() {
  server.handleClient();

  // if (pulseSensor.sawNewSample()) {
  //   if (pulseSensor.sawStartOfBeat()) {
  //     currentBPM = pulseSensor.getBeatsPerMinute();
  //     // Filtro simples para leituras válidas
  //     if (currentBPM > 40 && currentBPM < 200) {
  //       currentSpO2 = 98; // Valor de SpO2 fixo (placeholder)
  //     } else {
  //       currentBPM = 0; // Descarta BPMs fora da faixa esperada
  //     }
  //   }
  // }

  // // Lógica para detectar ausência do dedo (heuristicamente)
  // if (pulseSensor.getLatestSample() < (THRESHOLD / 2)) {
  //   currentBPM = 0;
  //   currentSpO2 = 0;
  // }
  
  // Envia os dados para o Firebase em intervalos definidos, se houver uma leitura válida
  // if (millis() - lastFirebaseUpdate > FIREBASE_UPDATE_INTERVAL && currentBPM => 0) {
  //   enviarDadosParaFirebase();
  //   lastFirebaseUpdate = millis();
  // }

  delay(20);
}

// --- Funções do Servidor Web ---

String gerarPagina(String titulo, String corpo) {
  // A biblioteca Chart.js foi removida daqui
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + titulo + "</title>"
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
  // Corpo da página Home simplificado, sem o canvas do gráfico.
  String corpo =
    "<div class='card data-display'>"
    "  <div class='data-item'>"
    "    <h2 id='bpmValue'>--</h2><p> BPM</p>"
    "  </div>"
    "  <div class='data-item'>"
    "    <h2 id='spo2Value'>--</h2><p> SpO2 (%)</p>"
    "  </div>"
    "</div>"
    "<div class='card'>" // Card adicional para informações
    "  <h2>Status do Dispositivo</h2>"
    "  <p>Monitorando leituras em tempo real.</p>"
    "  <p>Os dados estão sendo enviados para o Firebase a cada 5 segundos.</p>"
    "</div>"
    // Script simplificado para atualizar apenas os valores numéricos.
    "<script>"
    "  const bpmValue = document.getElementById('bpmValue');"
    "  const spo2Value = document.getElementById('spo2Value');"
    "  function updateData() {"
    "    fetch('/getData')"
    "      .then(response => response.json())"
    "      .then(data => {"
    "        bpmValue.textContent = data.bpm > 0 ? data.bpm : '--';"
    "        spo2Value.textContent = data.spo2 > 0 ? data.spo2 : '--';"
    "      });"
    "  }"
    "  setInterval(updateData, 2000);" // Atualiza os valores a cada 2 segundos
    "</script>";

  server.send(200, "text/html", gerarPagina("Monitor de Saúde", corpo));
}

void handleGetData() {
  // Retorna os dados atuais em formato JSON para a página Home.
  String json = "{";
  json += "\"bpm\":" + String(currentBPM) + ",";
  json += "\"spo2\":" + String(currentSpO2);
  json += "}";
  server.send(200, "application/json", json);
}

void enviarDadosParaFirebase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Desconectado. Abortando envio para o Firebase.");
    return;
  }

  // Define o caminho no Firebase.
  String path = logado && usuario != "" ? firebaseBasePath + "/" + usuario : firebaseBasePath + "/geral";
  // Cria um objeto JSON para enviar os dados.
  FirebaseJson jsonData;
  jsonData.set("bpm", String(currentBPM));
  jsonData.set("spo2", String(currentSpO2));
  //jsonData.set("timestamp", ".sv", "timestamp"); // Timestamp do servidor Firebase.
  Serial.printf("Enviando dados para Firebase em: %s\n", path.c_str());

  // Envia os dados usando pushJSON.
  // if (Firebase.pushJSON(fbdo, path, jsonData)) {
  //   Serial.println("--> Sucesso!");
  // } else {
  //   Serial.println("--> ERRO:");
  //   Serial.println(fbdo.errorReason());
  // }
}

// --- Funções de Perfil, Login, etc. ---
// (Estas funções permanecem inalteradas)
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
                "<p><label>Login: <input name='login' value='" + usuario + "' readonly></label></p>"
                "<p><label>Senha: <input type='password' name='senha' placeholder='Deixe em branco para não alterar'></label></p>"
                "<input type='submit' value='Salvar Perfil'>"
                "</form></div>";
 server.send(200, "text/html", gerarPagina("Perfil do Usuário", corpo));
}

void salvarPerfil() {
 nome = server.arg("nome");
 idade = server.arg("idade");
 email = server.arg("email");
 fotoURL = server.arg("foto");
 if (server.arg("senha") != "") {
    senha = server.arg("senha");
 }
 String corpo = "<div class='card'><p>Perfil salvo com sucesso!</p><a href='/perfil'><button>Voltar</button></a></div>";
 server.send(200, "text/html", gerarPagina("Perfil Salvo", corpo));
}

void handleHistoricoUsuario() {
 if (!logado) {
  server.sendHeader("Location", "/login");
  server.send(302, "text/plain", "");
  return;
 }
 String corpo = "<div class='card'><h2>Histórico do Usuário</h2>"
                "<p><strong>Nome:</strong> " + nome + "</p>"
                "<p><strong>Idade:</strong> " + idade + "</p>"
                "<p><strong>Email:</strong> " + email + "</p>"
              //  "<p><img src='" + fotoURL + "' alt='Foto do Perfil' width='100' style='border-radius:50%;'></p></div>"
                "<div class='card'><p>A visualização do histórico do Firebase pode ser implementada aqui.</p>"
                "<p>Os dados estão sendo salvos em: <strong>" + firebaseBasePath + "/" + usuario + "</strong></p></div>";
 server.send(200, "text/html", gerarPagina("Histórico do Perfil", corpo));
}

void handleML() {
 String corpo = "<div class='card'><h2>Machine Learning</h2><p>Acesse nosso modelo de Machine Learning no Google Colab:</p><a href='https://colab.research.google.com/drive/1PAZt0PLthIQHCQyrR3LrJpZ7q_Sgp8_0?usp=sharing' target='_blank'><button>Abrir Colab</button></a></div>";
 server.send(200, "text/html", gerarPagina("ML", corpo));
}

void handleLogout() {
 logado = false;
 usuario = "";
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
 server.sendHeader("Location", "/perfil");
 server.send(302, "text/plain", "");
}
void handleConfig() {
 String corpo = "<div class='card'><h2>Configurações</h2></div>";
 server.send(200, "text/html", gerarPagina("Configuração", corpo));
}
