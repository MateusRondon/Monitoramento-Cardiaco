#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Esp32";
const char* password = "2025mateus";

WebServer server(80);

const int PulseSensorPin = 4;
int leituraSuavizada = 0;
int oxigenacao = 98;
unsigned long tempoInicio = 0;
bool dedoPresente = false;

void handleHome();
void handleStatus();
void handleConfig();
void handlePerfil();
void handleAgendamento();
void handleLogout();
void handleBPM();

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
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleHome);
  server.on("/status", handleStatus);
  server.on("/config", handleConfig);
  server.on("/perfil", handlePerfil);
  server.on("/agendamento", handleAgendamento);
  server.on("/logout", handleLogout);
  server.on("/bpm", handleBPM);

  server.begin();
  Serial.println("Servidor iniciado.");
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

    int valorNormalizado = map(leituraBruta, 0, 1023, 60, 200);
    leituraSuavizada = (leituraSuavizada * 9 + valorNormalizado) / 10;

    oxigenacao = map(leituraBruta, 0, 1023, 90, 100);
  }

  Serial.print("BPM: ");
  Serial.print(leituraSuavizada);
  Serial.print(" | SpO2: ");
  Serial.println(oxigenacao);

  delay(1000);
}

void handleBPM() {
  unsigned long tempoAtual = dedoPresente ? (millis() - tempoInicio) / 1000 : 0;
  String json = "{\"bpm\":" + String(leituraSuavizada) + ",\"oxigenacao\":" + String(oxigenacao) + ",\"tempo\":" + String(tempoAtual) + "}";
  server.send(200, "application/json", json);
}

String gerarPagina(String titulo, String corpo) {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + titulo + "</title><style>"
         "body{font-family:Arial;background-color:#ffe6f0;color:#333;text-align:center;padding:20px}"
         "button{padding:10px 20px;font-size:16px;margin:10px;cursor:pointer;}"
         "</style></head><body><h1>" + titulo + "</h1>" + corpo +
         "<button onclick=\"location.href='/'\">Voltar para Home</button></body></html>";
}

void handleHome() {
  String pagina = R"rawliteral(
<!DOCTYPE html>
<html><head>
  <meta charset="UTF-8">
  <title>Monitor de Saúde</title>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
  <style>
    body { font-family: Arial; background-color: #ffe6f0; color: #333; margin: 0; }
    .menu { width: 60px; height: 100vh; background: #f8bbd0; position: fixed; transition: width 0.3s; }
    .menu:hover { width: 200px; }
    .menu a { display: flex; align-items: center; padding: 10px; text-decoration: none; color: #000; }
    .menu a i { width: 30px; text-align: center; }
    .menu span { opacity: 0; transition: 0.3s; }
    .menu:hover span { opacity: 1; }
    .content { margin-left: 80px; padding: 20px; }
    .card { background: #ffcdd2; padding: 20px; border-radius: 10px; margin: 20px auto; width: 300px; }
    .value { font-size: 2em; color: #e91e63; }
  </style>
</head><body>
<div class="menu">
  <a href="/"><i class="fas fa-home"></i><span>Home</span></a>
  <a href="/status"><i class="fas fa-heart"></i><span>Status</span></a>
  <a href="/config"><i class="fas fa-cog"></i><span>Configuração</span></a>
  <a href="/perfil"><i class="fas fa-user"></i><span>Perfil</span></a>
  <a href="/agendamento"><i class="fas fa-calendar-alt"></i><span>Agendamento</span></a>
  <a href="/logout"><i class="fas fa-sign-out-alt"></i><span>Logout</span></a>
</div>
<div class="content">
  <h1>Monitor de Saúde</h1>
  <div class="card"><h2>Batimentos Cardíacos</h2><div id="bpm" class="value">--</div></div>
  <div class="card"><h2>Oxigenação</h2><div id="spo2" class="value">--</div></div>
  <div class="card"><h2>Tempo de Leitura</h2><div id="tempo" class="value">--</div></div>
</div>
<script>
  function atualizar() {
    fetch("/bpm").then(r=>r.json()).then(d=>{
      document.getElementById("bpm").innerText = d.bpm;
      document.getElementById("spo2").innerText = d.oxigenacao + "%";
      document.getElementById("tempo").innerText = d.tempo + "s";
    });
  }
  setInterval(atualizar, 1000);
</script>
</body></html>
)rawliteral";
  server.send(200, "text/html", pagina);
}

void handleStatus() {
  String corpo = "<div class='card'><p>Histórico do Usuário:</p><p>Batimentos: 0 bpm</p><p>Oxigenação: 0%</p><p>Tempo de dedo: 0s</p></div>";
  server.send(200, "text/html", gerarPagina("Status", corpo));
}

void handleConfig() {
  String corpo = "<button onclick=\"document.body.classList.toggle('dark')\">Alterar Tema</button>";
  server.send(200, "text/html", gerarPagina("Configurações", corpo));
}

void handlePerfil() {
  String corpo = "<p>Nome: Usuário de Teste</p><p>ID: 001</p>";
  server.send(200, "text/html", gerarPagina("Perfil", corpo));
}

void handleAgendamento() {
  String corpo = "<p>Escolha sua especialidade:</p>"
                 "<button onclick=\"alert('Clínico agendado!')\">Clínico Geral</button>"
                 "<button onclick=\"alert('Ortopedista agendado!')\">Ortopedista</button>";
  server.send(200, "text/html", gerarPagina("Agendamento Médico", corpo));
}

void handleLogout() {
  String corpo = "<p>Logout em desenvolvimento...</p>";
  server.send(200, "text/html", gerarPagina("Logout", corpo));
}
