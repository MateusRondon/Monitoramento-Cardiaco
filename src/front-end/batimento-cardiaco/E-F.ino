#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>
// --- Credenciais do Wi-Fi ---
const char* ssid = "Esp32";
const char* password = "2025mateus";

// --- Servidor na porta 80 ---
WebServer server(80);

// --- Variáveis Globais ---
// Sensores (tempo real)
const int PulseSensorPin = 4;
int leituraSuavizada = 0;
int oxigenacao = 98;
unsigned long tempoInicio = 0;
bool dedoPresente = false;

// Dados do Perfil do Usuário
String userName = "Não definido";
String userEmail = "Não definido";
int userAge = 0;

// Dados da Última Medição Válida (para a página de Status)
int lastBpm = 0;
int lastSpo2 = 0;
unsigned long lastReadingTimestamp = 0;
typedef struct struct_message {
    int bpm;
    int spo2;
} struct_message;

// Crie uma instância da estrutura
struct_message myData;
// --- Coloque aqui o MAC Address do seu ESP32 #2 (Gateway) ---
uint8_t broadcastAddress[] = {0x3C, 0x71, 0xBF, 0xF9, 0xB4, 0x74}; // SUBSTITUA PELO SEU MAC
// Variável para o status do envio
esp_now_peer_info_t peerInfo;
// Callback que é chamada quando o dado é enviado
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nStatus do último envio: ");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso!" : "Falha no Envio");
// }
// --- Declaração das Funções (Handlers) ---
void handleHome();
void handleStatus();
void handleML(); // Nova função para a página M-L
void handlePerfil();
void handleLogout();
void handleBPM();
void handleStatusData(); // Novo endpoint de dados para a página de Status

// Armazena a página HTML principal na memória Flash (PROGMEM) para economizar RAM
const char PAGINA_HOME[] PROGMEM = R"rawliteral(
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
    .card { background: #ffcdd2; padding: 20px; border-radius: 10px; margin: 20px auto; max-width: 400px; text-align: center; }
    .value { font-size: 2em; color: #e91e63; }
  </style>
</head><body>
<div class="menu">
  <a href="/"><i class="fas fa-home"></i><span>Home</span></a>
  <a href="/status"><i class="fas fa-history"></i><span>Status</span></a>
  <a href="/ML"><i class="fas fa-brain"></i><span>M-L</span></a>
  <a href="/perfil"><i class="fas fa-user"></i><span>Perfil</span></a>
  <a href="/logout"><i class="fas fa-sign-out-alt"></i><span>Logout</span></a>
</div>
<div class="content">
  <h1>Monitor de Saúde</h1>
  <div class="card"><h2>Batimentos Cardíacos</h2><div id="bpm" class="value">--</div></div>
  <div class="card"><h2>Oxigenação</h2><div id="spo2" class="value">--%</div></div>
  <div class="card"><h2>Tempo de Leitura</h2><div id="tempo" class="value">--s</div></div>
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


void setup() {
  Serial.begin(115200);
  analogReadResolution(10);

  WiFi.begin(ssid, password);
  Serial.print(F("Conectando ao Wi-Fi"));
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println(F("\nWi-Fi conectado!"));
  Serial.print(F("IP: "));
  Serial.println(WiFi.localIP());
  //WIFI -> MacAddress.
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("======================================");
  Serial.print("Endereço MAC deste ESP32: ");
  Serial.println(WiFi.macAddress());
  // Rotas do servidor
  server.on(F("/"), handleHome);
  server.on(F("/status"), handleStatus);
  server.on(F("/status_data"), handleStatusData); // Rota para os dados do status
  server.on(F("/ML"), handleML);                   // Nova rota M-L
  server.on(F("/perfil"), handlePerfil);
  server.on(F("/logout"), handleLogout);
  server.on(F("/bpm"), handleBPM);

  server.begin();
  Serial.println(F("Servidor iniciado."));
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }

  // Registra a função de callback do envio
  // esp_now_register_send_cb(OnDataSent);

  // Registra o outro ESP32 (peer) para comunicação
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Adiciona o peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha ao adicionar o peer");
    return;
  }
  delay(1000);
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

    // Salva a última medição válida para a página de Status
    lastBpm = leituraSuavizada;
    lastSpo2 = oxigenacao;
    lastReadingTimestamp = millis();
    
    // --- INÍCIO DA ADIÇÃO ---
    // Prepara os dados na estrutura para o envio
    myData.bpm = lastBpm;
    myData.spo2 = lastSpo2;

    // Envia a mensagem via ESP-NOW para o outro ESP32
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    // --- FIM DA ADIÇÃO ---
  }
  // Aumentei o delay para 1 segundo para dar tempo ao envio e evitar sobrecarga
  delay(1000); 
}

// Handler para enviar dados em tempo real via JSON
void handleBPM() {
  unsigned long tempoAtual = dedoPresente ? (millis() - tempoInicio) / 1000 : 0;
  String json = "{\"bpm\":" + String(leituraSuavizada) + ",\"oxigenacao\":" + String(oxigenacao) + ",\"tempo\":" + String(tempoAtual) + "}";
  server.send(200, F("application/json"), json);
}

// Handler para enviar os dados da ÚLTIMA medição válida
void handleStatusData() {
  unsigned long tempoDesdeLeitura = 0;
  if (lastReadingTimestamp > 0) {
    tempoDesdeLeitura = (millis() - lastReadingTimestamp) / 1000;
  }
  String json = "{\"lastBpm\":" + String(lastBpm) + 
                ",\"lastSpo2\":" + String(lastSpo2) + 
                ",\"tempoDesdeLeitura\":" + String(tempoDesdeLeitura) + "}";
  server.send(200, F("application/json"), json);
}

// Função otimizada para gerar o template das páginas HTML
String gerarPagina(const String& titulo, const String& corpo) {
  return String(F("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>")) + titulo +
         String(F("</title><style>")) +
         String(F("body{font-family:Arial;background-color:#ffe6f0;color:#333;text-align:center;padding:20px}")) +
         String(F(".card{background:#ffcdd2;padding:20px;border-radius:10px;margin:20px auto;max-width:400px;text-align:left;}")) +
         String(F("button,input[type=submit]{border:none; border-radius:5px; background-color:#f8bbd0; padding:10px 20px;font-size:16px;margin:10px 5px;cursor:pointer;}")) +
         String(F("input{width:calc(100% - 20px);padding:8px;margin-bottom:10px;}")) +
         String(F(".value{font-size:2em;color:#e91e63;text-align:center;}")) +
         String(F("</style></head><body><h1>")) + titulo + String(F("</h1>")) + corpo +
         String(F("<br><button onclick=\"location.href='/'\">Voltar para Home</button></body></html>"));
}

// Handler para a página principal (Home)
void handleHome() {
  server.send_P(200, "text/html", PAGINA_HOME);
}

// Handler da página de Status (agora dinâmica)
void handleStatus() {
  String corpo = String(F("<div class='card'>")) +
                 F("<h2>Última Medição Registrada</h2>") +
                 F("<p>Esta página é atualizada a cada 10 segundos com a última medição válida detectada pelo sensor.</p><hr>") +
                 F("<p><b>Batimentos:</b></p><div id='last_bpm' class='value'>--</div>") +
                 F("<p><b>Oxigenação:</b></p><div id='last_spo2' class='value'>--</div>") +
                 F("<p><b>Medição realizada há:</b></p><div id='tempo' class='value'>--</div>") +
                 F("</div>") +
                 // Script para buscar os dados
                 F("<script>") +
                 F("function atualizarStatus() {") +
                 F("  fetch('/status_data').then(r=>r.json()).then(d=>{") +
                 F("    document.getElementById('last_bpm').innerText = d.lastBpm + ' BPM';") +
                 F("    document.getElementById('last_spo2').innerText = d.lastSpo2 + '%';") +
                 F("    document.getElementById('tempo').innerText = d.tempoDesdeLeitura + 's atrás';") +
                 F("  });") +
                 F("}") +
                 F("setInterval(atualizarStatus, 10000);") + // Atualiza a cada 10 segundos
                 F("window.onload = atualizarStatus;") + // Chama na primeira vez que carrega a página
                 F("</script>");
  server.send(200, F("text/html"), gerarPagina(F("Status da Medição"), corpo));
}


// Handler para a nova página M-L
void handleML() {
  String corpo = String(F("<div class='card'>")) +
                 F("<p>Esta área será destinada a funcionalidades de Machine Learning.</p>") +
                 // O onclick exibe um alerta, o href="#" impede a página de recarregar.
                 F("<button onclick=\"alert('O link para o Colab será inserido aqui em breve!');\" href=\"#\">Colab.</button>") +
                 F("</div>");
  server.send(200, F("text/html"), gerarPagina(F("M-L"), corpo));
}

// Handler da página de Perfil (sem alterações)
void handlePerfil() {
  if (server.method() == HTTP_POST) {
    userName = server.arg("nome");
    userEmail = server.arg("email");
    userAge = server.arg("idade").toInt();
    String corpo = String(F("<div class='card'>")) +
                   F("<h3>Dados salvos com sucesso!</h3>") +
                   F("<p><b>Nome:</b> ") + userName + F("</p>") +
                   F("<p><b>E-mail:</b> ") + userEmail + F("</p>") +
                   F("<p><b>Idade:</b> ") + String(userAge) + F("</p>") +
                   F("</div>");
    server.send(200, F("text/html"), gerarPagina(F("Perfil Atualizado"), corpo));
  } else {
    String corpo = String(F("<div class='card'>")) +
                   F("<h3>Perfil do Usuário</h3>") +
                   F("<p><b>Nome:</b> ") + userName + F("</p>") +
                   F("<p><b>E-mail:</b> ") + userEmail + F("</p>") +
                   F("<p><b>Idade:</b> ") + String(userAge) + F(" anos</p>") +
                   F("</div>") +
                   F("<h3>Editar Perfil</h3>") +
                   F("<form action='/perfil' method='POST' class='card'>") +
                   F("<label for='nome'>Nome:</label><br><input type='text' name='nome' required><br>") +
                   F("<label for='email'>Email:</label><br><input type='email' name='email' required><br>") +
                   F("<label for='idade'>Idade:</label><br><input type='number' name='idade' required><br><br>") +
                   F("<input type='submit' value='Salvar Alterações'>") +
                   F("</form>");
    server.send(200, F("text/html"), gerarPagina(F("Perfil"), corpo));
  }
}

// Handler da página de Logout
void handleLogout() {
  String corpo = F("<p>Logout em desenvolvimento...</p>");
  server.send(200, F("text/html"), gerarPagina(F("Logout"), corpo));
}