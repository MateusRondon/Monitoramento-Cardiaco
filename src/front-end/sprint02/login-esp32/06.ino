#include <WiFi.h>
#include <WebServer.h>

// Credenciais do Wi-Fi
const char* ssid = "Esp32";
const char* password = "2025mateus";

// Servidor HTTP na porta 80
WebServer server(80);

// Variáveis de login
String savedUser = "admin";
String savedPass = "1234";
bool userLoggedIn = false;

// Setup
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

  // Rotas
  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", handleLogout);
  server.on("/register", HTTP_POST, handleRegister);
  server.on("/home", handleHome);

  server.begin();
  Serial.println("Servidor iniciado.");
}

void loop() {
  server.handleClient();
}

// Página Inicial (Login e Registro)
void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html><html><head>
  <meta charset="UTF-8">
  <title>Login</title>
  <style>
    body { background-color: #ffe6f0; font-family: Arial; text-align: center; padding: 50px; }
    input { padding: 10px; margin: 5px; width: 200px; }
    button { padding: 10px 20px; margin: 10px; }
  </style>
</head><body>
  <h2>Login</h2>
  <form action="/login" method="post">
    <input name="username" placeholder="Usuário" required><br>
    <input name="password" type="password" placeholder="Senha" required><br>
    <button type="submit">Entrar</button>
  </form>
  <h3>Registrar</h3>
  <form action="/register" method="post">
    <input name="newuser" placeholder="Novo Usuário" required><br>
    <input name="newpass" type="password" placeholder="Nova Senha" required><br>
    <button type="submit">Registrar</button>
  </form>
</body></html>
)rawliteral";

  server.send(200, "text/html", page);
}

// Processa login
void handleLogin() {
  String user = server.arg("username");
  String pass = server.arg("password");

  if (user == savedUser && pass == savedPass) {
    userLoggedIn = true;
    server.sendHeader("Location", "/home");
    server.send(303);
  } else {
    server.send(200, "text/html", "<h3>Login falhou. <a href='/'>Tente novamente</a></h3>");
  }
}

// Processa registro
void handleRegister() {
  savedUser = server.arg("newuser");
  savedPass = server.arg("newpass");
  userLoggedIn = true;
  server.sendHeader("Location", "/home");
  server.send(303);
}

// Logout
void handleLogout() {
  userLoggedIn = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

// Página protegida (Home)
void handleHome() {
  if (!userLoggedIn) {
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }

  String page = R"rawliteral(
<!DOCTYPE html><html><head>
  <meta charset="UTF-8">
  <title>Home</title>
  <style>
    body { font-family: Arial; background-color: #ffe6f0; color: #333; text-align: center; padding: 20px; }
    .card { background: #ffcdd2; padding: 20px; margin: 20px auto; width: 300px; border-radius: 10px; }
    button { padding: 10px 20px; }
  </style>
</head><body>
  <h1>Bem-vindo(a)</h1>
  <div class="card"><p>Usuário autenticado com sucesso.</p></div>
  <form action="/logout" method="get"><button type="submit">Logout</button></form>
</body></html>
)rawliteral";

  server.send(200, "text/html", page);
}
