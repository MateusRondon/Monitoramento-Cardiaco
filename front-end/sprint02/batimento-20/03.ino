#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <MAX30105.h>
#include <heartRate.h>
// Wi-Fi
const char* ssid = "Esp32";
const char* password = "2025mateus";
// Web Server
WebServer server(80);
// Sensor
MAX30105 particleSensor;
int leituraSuavizada = 0;
int oxigenacao = 98;
unsigned long tempoInicio = 0;
bool dedoPresente = false;
struct Dado {
  int bpm;
  int spo2;
  unsigned long tempo;
};
#define MAX_DADOS 20
Dado historico[MAX_DADOS];
int indiceHistorico = 0;
// Perfil e Autenticação
String nome = "";
String idade = "";
String email = "";
String fotoURL = "";
String usuario = "";
String senha = "";
bool logado = false;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());
  max();
  server.on("/", handleHome);
  server.on("/status", handleStatus);
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
  server.begin();
}
void max() {
  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}
void loop() {
  server.handleClient();
  if (particleSensor.available()) {
    long irValue = particleSensor.getIR();
    if (irValue < 50000) {
      leituraSuavizada = 0;
      oxigenacao = 0;
      dedoPresente = false;
      tempoInicio = 0;
    } else {
      if (!dedoPresente) {
        tempoInicio = millis();
        dedoPresente = true;
      }
      int bpm = checkForBeat(irValue);
      if (bpm > 0) {
        leituraSuavizada = bpm;
      }
      oxigenacao = 98;
      historico[indiceHistorico] = { leituraSuavizada, oxigenacao, (millis() - tempoInicio) / 1000 };
      indiceHistorico = (indiceHistorico + 1) % MAX_DADOS;
    }
  }
  delay(500);
}
String gerarPagina(String titulo, String corpo) {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + titulo + "</title><style>"
                                                                               "body{font-family:Arial;margin:0;padding:0;display:flex;transition:background 0.3s ease;}"
                                                                               ".menu{position:fixed;top:0;left:0;width:200px;height:100vh;background:#f06292;color:white;padding-top:20px;display:flex;flex-direction:column;align-items:start;gap:15px;padding-left:15px;z-index:1;}"
                                                                               ".menu a{color:white;text-decoration:none;font-weight:bold;display:flex;align-items:center;gap:8px;font-size:16px;}"
                                                                               ".content{margin-left:200px;padding:20px;width:100%;}"
                                                                               ".card{background:#f8bbd0;padding:20px;border-radius:15px;max-width:700px;margin:20px auto;box-shadow:0 4px 8px rgba(0,0,0,0.1);}"
                                                                               "button,input[type='submit'],select{padding:10px 20px;font-size:16px;margin:10px;border:none;border-radius:25px;background-color:#e91e63;color:white;cursor:pointer;box-shadow:0 4px 6px rgba(0,0,0,0.2);transition:background-color 0.3s ease;}"
                                                                               "button:hover,input[type='submit']:hover{background-color:#d81b60;}"
                                                                               "input[type='text'],input[type='password']{padding:10px;border-radius:10px;border:1px solid #ccc;width:90%;}"
                                                                               "table{margin:0 auto;border-collapse:collapse;}th,td{border:1px solid #ccc;padding:8px;}"
                                                                               ".dark{background:#222;color:white;}.dark .card{background:#333;}.dark .menu{background:#111;}"
                                                                               ".blue{background:#f6f8fa;color:#24292e;}.blue .card{background:#ffffff;}.blue .menu{background:#24292e;color:white;}"
                                                                               ".red{background:#ffebee;color:#b71c1c;}.red .card{background:#ffcdd2;}.red .menu{background:#b71c1c;color:white;}"
                                                                               ".purple{background:#f3e5f5;color:#4a148c;}.purple .card{background:#ce93d8;}.purple .menu{background:#4a148c;color:white;}"
                                                                               "</style><script>"
                                                                               "function setTheme(theme){document.body.className='';document.body.classList.add(theme);localStorage.setItem('theme', theme);}"
                                                                               "window.onload=function(){var theme=localStorage.getItem('theme');if(theme){setTheme(theme);var select=document.getElementById('themeSelect');if(select)select.value=theme;}}"
                                                                               "</script></head><body>"
                                                                               "<div class='menu'>"
                                                                               "<a href='/'>&#127968; Home</a>"
                                                                               "<a href='/historicoUsuario'>&#128196; Histórico</a>"
                                                                               "<a href='/config'>&#9881; Config</a>"
                                                                               "<a href='/perfil'>&#128100; Perfil</a>"
                                                                               "<a href='/ml'>&#129302; ML</a>"
                                                                               "<a href='/logout'>&#128682; Logout</a>"
                                                                               "</div>"
                                                                               "<div class='content'><h1>"
         + titulo + "</h1>" + corpo + "</div></body></html>";
}
void handleHome() {
  int ultimoIndice = (indiceHistorico - 1 + MAX_DADOS) % MAX_DADOS;
  Dado dadoAtual = historico[ultimoIndice];
  String corpo = "<div class='card'><h2>Status Atual</h2>"
                 "<p><strong>BPM:</strong> "
                 + String(dadoAtual.bpm) + "</p>"
                                           "<p><strong>SpO2:</strong> "
                 + String(dadoAtual.spo2) + "%</p>"
                                            "<p><strong>Tempo com Dedo:</strong> "
                 + String(dadoAtual.tempo) + " segundos</p></div>";
  // Adiciona gráficos
  corpo += "<div class='card'><h2>Gráficos em Tempo Real</h2>"
           "<div id='bpmChart' style='width:100%;height:200px;'></div>"
           "<div id='spo2Chart' style='width:100%;height:200px;'></div>"
           "<script>"
           "const ctxBPM = document.createElement('canvas').getContext('2d');"
           "const ctxSpO2 = document.createElement('canvas').getContext('2d');"
           "let bpmData = [];"
           "let spo2Data = [];"
           "let timeData = [];"
           "let maxDataPoints = 20;"
           "let updateInterval = 1000;"
           "let now = new Date().getTime();"
           "let lastUpdate = now - updateInterval;"
           "let xScale = 40;"
           "let yScaleBPM = 2;"
           "let yScaleSpO2 = 2;"
           "let offsetBPM = 100;"
           "let offsetSpO2 = 100;"
           "let widthBPM = document.getElementById('bpmChart').clientWidth;"
           "let heightBPM = document.getElementById('bpmChart').clientHeight;"
           "let widthSpO2 = document.getElementById('spo2Chart').clientWidth;"
           "let heightSpO2 = document.getElementById('spo2Chart').clientHeight;"
           "ctxBPM.canvas.width = widthBPM;"
           "ctxBPM.canvas.height = heightBPM;"
           "ctxSpO2.canvas.width = widthSpO2;"
           "ctxSpO2.canvas.height = heightSpO2;"
           "document.getElementById('bpmChart').appendChild(ctxBPM.canvas);"
           "document.getElementById('spo2Chart').appendChild(ctxSpO2.canvas);"
           "function updateCharts() {"
           "  now = new Date().getTime();"
           "  if (now - lastUpdate >= updateInterval) {"
           "    // Atualiza dados"
           "    fetch('/getBPMData')"
           "      .then(response => response.json())"
           "      .then(data => {"
           "        bpmData.push(data.bpm);"
           "        spo2Data.push(data.spo2);"
           "        timeData.push(data.time);"
           "        if (bpmData.length > maxDataPoints) {"
           "          bpmData.shift();"
           "          spo2Data.shift();"
           "          timeData.shift();"
           "        }"
           "        drawCharts();"
           "      });"
           "    lastUpdate = now;"
           "  }"
           "  requestAnimationFrame(updateCharts);"
           "}"
           "function drawCharts() {"
           "  // Limpa os gráficos"
           "  ctxBPM.clearRect(0, 0, widthBPM, heightBPM);"
           "  ctxSpO2.clearRect(0, 0, widthSpO2, heightSpO2);"
           "  // Desenha eixos"
           "  ctxBPM.beginPath();"
           "  ctxBPM.moveTo(50, 0);"
           "  ctxBPM.lineTo(50, heightBPM);"
           "  ctxBPM.moveTo(50, heightBPM);"
           "  ctxBPM.lineTo(widthBPM, heightBPM);"
           "  ctxBPM.strokeStyle = '#666';"
           "  ctxBPM.stroke();"
           "  // Desenha linha BPM"
           "  ctxBPM.beginPath();"
           "  ctxBPM.moveTo(50, heightBPM - (bpmData[0] * yScaleBPM) - offsetBPM);"
           "  for (let i = 1; i < bpmData.length; i++) {"
           "    ctxBPM.lineTo(50 + (i * xScale), heightBPM - (bpmData[i] * yScaleBPM) - offsetBPM);"
           "  }"
           "  ctxBPM.strokeStyle = '#e91e63';"
           "  ctxBPM.stroke();"
           "  // Repete para SpO2"
           "  ctxSpO2.beginPath();"
           "  ctxSpO2.moveTo(50, 0);"
           "  ctxSpO2.lineTo(50, heightSpO2);"
           "  ctxSpO2.moveTo(50, heightSpO2);"
           "  ctxSpO2.lineTo(widthSpO2, heightSpO2);"
           "  ctxSpO2.strokeStyle = '#666';"
           "  ctxSpO2.stroke();"
           "  ctxSpO2.beginPath();"
           "  ctxSpO2.moveTo(50, heightSpO2 - (spo2Data[0] * yScaleSpO2) - offsetSpO2);"
           "  for (let i = 1; i < spo2Data.length; i++) {"
           "    ctxSpO2.lineTo(50 + (i * xScale), heightSpO2 - (spo2Data[i] * yScaleSpO2) - offsetSpO2);"
           "  }"
           "  ctxSpO2.strokeStyle = '#4CAF50';"
           "  ctxSpO2.stroke();"
           "}"
           "updateCharts();"
           "</script></div>";
  server.send(200, "text/html", gerarPagina("Home", corpo));
}
void handleStatus() {
  String corpo = "<div class='card'><h2>Status</h2><table><tr><th>BPM</th><th>SpO2</th><th>Tempo (s)</th></tr>";
  for (int i = 0; i < MAX_DADOS; i++) {
    if (historico[i].bpm > 0) {
      corpo += "<tr><td>" + String(historico[i].bpm) + "</td><td>" + String(historico[i].spo2) + "</td><td>" + String(historico[i].tempo) + "</td></tr>";
    }
  }
  corpo += "</table></div>";
  server.send(200, "text/html", gerarPagina("Status", corpo));
}
void handleConfig() {
  String corpo = "<div class='card'><h2>Configurações</h2>"
                 "<p><label>Escolher Tema: "
                 "<select id='themeSelect' onchange='setTheme(this.value)'>"
                 "<option value=''>Claro</option>"
                 "<option value='dark'>Escuro</option>"
                 "<option value='blue'>Azul (GitHub)</option>"
                 "<option value='red'>Vermelho</option>"
                 "<option value='purple'>Roxo (Nubank)</option>"
                 "</select></label></p></div>";
  server.send(200, "text/html", gerarPagina("Configuração", corpo));
}
void handlePerfil() {
  if (!logado) {
    server.sendHeader("Location", "/login");
    server.send(302, "text/plain", "");
    return;
  }
  String corpo = "<div class='card'><form action='/salvarPerfil' method='POST'>"
                 "<p><label>Nome: <input name='nome' value='"
                 + nome + "'></label></p>"
                          "<p><label>Idade: <input name='idade' value='"
                 + idade + "'></label></p>"
                           "<p><label>Email: <input name='email' value='"
                 + email + "'></label></p>"
                           "<p><label>Foto URL: <input name='foto' value='"
                 + fotoURL + "'></label></p>"
                             "<p><label>Login: <input name='login' value='"
                 + usuario + "'></label></p>"
                             "<p><label>Senha: <input type='password' name='senha' value='"
                 + senha + "'></label></p>"
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
  String corpo = "<div class='card'><h2>Histórico do Usuário</h2>"
                 "<p><strong>Nome:</strong> "
                 + nome + "</p>"
                          "<p><strong>Idade:</strong> "
                 + idade + "</p>"
                           "<p><strong>Email:</strong> "
                 + email + "</p>"
                           "<p><img src='"
                 + fotoURL + "' alt='Foto' width='100'></p></div>";
  corpo += "<div class='card'><h3>Histórico de Batimentos</h3><table><tr><th>BPM</th><th>SpO2</th><th>Tempo (s)</th></tr>";
  for (int i = 0; i < MAX_DADOS; i++) {
    if (historico[i].bpm > 0) {
      corpo += "<tr><td>" + String(historico[i].bpm) + "</td><td>" + String(historico[i].spo2) + "</td><td>" + String(historico[i].tempo) + "</td></tr>";
    }
  }
  corpo += "</table></div>";
  server.send(200, "text/html", gerarPagina("Histórico do Perfil", corpo));
}
void handleML() {
  String corpo = "<div class='card'><h2>Machine Learning</h2><p>Funcionalidade em desenvolvimento.</p></div>";
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