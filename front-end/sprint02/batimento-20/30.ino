#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi
const char* ssid = "Esp32";
const char* password = "2025mateus";

// Web Server
WebServer server(80);

// Sensor
const int PulseSensorPin = 4;
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

String nome = "";
String idade = "";
String email = "";
String fotoURL = "";

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());

  server.on("/", handleHome);
  server.on("/status", handleStatus);
  server.on("/config", handleConfig);
  server.on("/perfil", handlePerfil);
  server.on("/salvarPerfil", HTTP_POST, salvarPerfil);
  server.on("/historicoUsuario", handleHistoricoUsuario);
  server.on("/ml", handleML);
  server.on("/logout", handleLogout);
  server.begin();
}

void loop() {
  server.handleClient();

  int leituraBruta = analogRead(PulseSensorPin);

  if (leituraBruta < 50) {
    leituraSuavizada = 0;
    oxigenacao = 0;
    dedoPresente = false;
    tempoInicio = 0;
  } else {
    if (!dedoPresente) {
      tempoInicio = millis();
      dedoPresente = true;
    }
    leituraSuavizada = random(60, 101);
    oxigenacao = random(90, 101);

    historico[indiceHistorico] = { leituraSuavizada, oxigenacao, (millis() - tempoInicio) / 1000 };
    indiceHistorico = (indiceHistorico + 1) % MAX_DADOS;
  }

  delay(5000);
}

String gerarPagina(String titulo, String corpo) {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + titulo + "</title><style>"
         "body{font-family:Arial;margin:0;padding:0;display:flex;transition:background 0.3s ease;}"
         ".menu{position:fixed;top:0;left:0;width:200px;height:100vh;background:#f06292;color:white;padding-top:20px;display:flex;flex-direction:column;align-items:start;gap:15px;padding-left:15px;z-index:1;}"
         ".menu a{color:white;text-decoration:none;font-weight:bold;display:flex;align-items:center;gap:8px;font-size:16px;}"
         ".content{margin-left:200px;padding:20px;width:100%;}"
         ".card{background:#f8bbd0;padding:20px;border-radius:15px;max-width:700px;margin:20px auto;box-shadow:0 4px 8px rgba(0,0,0,0.1);}"
         "button,input[type='submit']{padding:10px 20px;font-size:16px;margin:10px;border:none;border-radius:25px;background-color:#e91e63;color:white;cursor:pointer;box-shadow:0 4px 6px rgba(0,0,0,0.2);transition:background-color 0.3s ease;}"
         "button:hover,input[type='submit']:hover{background-color:#d81b60;}"
         "table{margin:0 auto;border-collapse:collapse;}th,td{border:1px solid #ccc;padding:8px;}"
         ".dark{background:#222;color:white;}"
         ".dark .card{background:#333;}"
         "</style><script>function toggleTheme(){document.body.classList.toggle('dark');}</script></head><body>"
         "<div class='menu'>"
         "<a href='/'>&#127968; Home</a>"
         "<a href='/historicoUsuario'>&#128196; Histórico</a>"
         "<a href='/config'>&#9881; Config</a>"
         "<a href='/perfil'>&#128100; Perfil</a>"
         "<a href='/ml'>&#129302; ML</a>"
         "<a href='/logout'>&#128682; Logout</a>"
         "</div>"
         "<div class='content'><h1>" + titulo + "</h1>" + corpo + "</div>"
         "</body></html>";
}

void handleHome() {
  int ultimoIndice = (indiceHistorico - 1 + MAX_DADOS) % MAX_DADOS;
  Dado dadoAtual = historico[ultimoIndice];

  String corpo = "<div class='card'><h2>Status Atual</h2>"
                 "<p><strong>BPM:</strong> " + String(dadoAtual.bpm) + "</p>"
                 "<p><strong>SpO2:</strong> " + String(dadoAtual.spo2) + "%</p>"
                 "<p><strong>Tempo com Dedo:</strong> " + String(dadoAtual.tempo) + " segundos</p></div>";
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
  String corpo = "<div class='card'><h2>Configurações</h2><button onclick='toggleTheme()'>Alterar Tema</button></div>";
  server.send(200, "text/html", gerarPagina("Configuração", corpo));
}

void handlePerfil() {
  String corpo = "<div class='card'><form action='/salvarPerfil' method='POST'>"
                 "<p><label>Nome: <input name='nome' value='" + nome + "'></label></p>"
                 "<p><label>Idade: <input name='idade' value='" + idade + "'></label></p>"
                 "<p><label>Email: <input name='email' value='" + email + "'></label></p>"
                 "<p><label>Foto URL: <input name='foto' value='" + fotoURL + "'></label></p>"
                 "<input type='submit' value='Salvar'>"
                 "</form></div>"
                 "<div class='card'><h3>Resumo do Perfil</h3>"
                 "<p><strong>Nome:</strong> " + nome + "</p>"
                 "<p><strong>Idade:</strong> " + idade + "</p>"
                 "<p><strong>Email:</strong> " + email + "</p>"
                 "<p><img src='" + fotoURL + "' alt='Foto' width='100'></p>"
                 "<button onclick=\"location.href='/historicoUsuario'\">Ver Histórico</button></div>";
  server.send(200, "text/html", gerarPagina("Perfil do Usuário", corpo));
}

void salvarPerfil() {
  nome = server.arg("nome");
  idade = server.arg("idade");
  email = server.arg("email");
  fotoURL = server.arg("foto");

  String corpo = "<div class='card'><p>Perfil salvo com sucesso!</p><button onclick=\"location.href='/perfil'\">Voltar</button></div>";
  server.send(200, "text/html", gerarPagina("Perfil Salvo", corpo));
}

void handleHistoricoUsuario() {
  String corpo = "<div class='card'><h2>Histórico do Usuário</h2>"
                 "<p><strong>Nome:</strong> " + nome + "</p>"
                 "<p><strong>Idade:</strong> " + idade + "</p>"
                 "<p><strong>Email:</strong> " + email + "</p>"
                 "<p><img src='" + fotoURL + "' alt='Foto' width='100'></p></div>";

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
  String corpo = "<div class='card'><h2>Logout</h2><p>Você foi desconectado com sucesso (simulado).</p></div>";
  server.send(200, "text/html", gerarPagina("Logout", corpo));
}
