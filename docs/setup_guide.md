# 📖 Guia de Setup Completo

## Pré-requisitos

### Hardware
- ESP32 Dev Board
- Sensor MAX30100
- Cabos USB para programação
- Jumpers para conexão

### Software
- Arduino IDE 1.8.0 ou superior
- Git
- Conta no Blynk.io

## Passo 1: Instalar Arduino IDE

1. Baixar em: https://www.arduino.cc/en/software
2. Instalar seguindo o instalador

## Passo 2: Configurar ESP32 no Arduino IDE

1. **Abrir Preferences:**
   - Arduino IDE → File → Preferences

2. **Adicionar URL do ESP32:**
   - Em "Additional Board Manager URLs" adicione:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

3. **Instalar Board Manager:**
   - Tools → Board → Boards Manager
   - Procurar por "ESP32"
   - Instalar "ESP32 by Espressif Systems"

## Passo 3: Instalar Bibliotecas Necessárias

1. **Sketch → Include Library → Manage Libraries**
2. Instalar as seguintes:
   - `Blynk` (versão 1.0.0+)
   - `MAX30100lib`
   - `WiFi` (geralmente já vem instalado)

```
Sketch → Include Library → Manage Libraries
Buscar e instalar:
- Blynk
- MAX30100lib
- ArduinoJson (opcional, para configurações)
```

## Passo 4: Conectar Hardware

### Pinagem ESP32 → MAX30100

| ESP32 | MAX30100 |
|-------|----------|
| GND   | GND      |
| 3.3V  | VCC      |
| GPIO 21 (SDA) | SDA      |
| GPIO 22 (SCL) | SCL      |

### Diagrama de Conexão

```
     ESP32          MAX30100
    +-----+        +-------+
    |     |        |       |
    | GND |--------|GND    |
    |     |        |       |
    |3.3V |--------|VCC    |
    |     |        |       |
    |GPIO21|-------|SDA    |
    |     |        |       |
    |GPIO22|-------|SCL    |
    |     |        |       |
    +-----+        +-------+
```

## Passo 5: Configurar Blynk IoT

1. **Criar Conta:**
   - Acesse: https://blynk.io
   - Criar conta com email

2. **Gerar Auth Token:**
   - Criar novo projeto
   - Nome: "Monitor Cardíaco"
   - Device: "ESP32"
   - Copiar o token gerado

3. **Configurar Widgets:**
   - Value Display para BPM
   - Value Display para SpO2
   - Gauge para alertas
   - Graph para histórico

## Passo 6: Configurar Código

1. **Editar credenciais em `src/main.cpp`:**

```cpp
// WiFi
const char* ssid = "SEU_WIFI_SSID";
const char* password = "SUA_SENHA_WIFI";

// Blynk
const char* BLYNK_TEMPLATE_ID = "TMPxxxxxx";
const char* BLYNK_TEMPLATE_NAME = "Cardio Monitor";
const char* BLYNK_AUTH_TOKEN = "SEU_TOKEN_AQUI";
```

2. **Selecionar Board:**
   - Tools → Board → ESP32 Dev Module

3. **Selecionar Porta:**
   - Tools → Port → COM3 (ou a porta do seu ESP32)

## Passo 7: Fazer Upload

1. Conectar ESP32 via USB
2. Sketch → Upload (ou Ctrl+U)
3. Aguardar mensagem "Upload complete"

## Passo 8: Verificar Funcionamento

1. **Monitor Serial:**
   - Abrir Tools → Serial Monitor
   - Baud rate: 115200
   - Verificar leitura do sensor

2. **Dashboard Blynk:**
   - Abrir app Blynk no celular
   - Verificar se dados aparecem em tempo real

## Troubleshooting

### Problema: "Placa não encontrada"
**Solução:**
- Verificar conexão USB
- Instalar drivers CH340 (alguns ESP32 usam esse chip)
- Reiniciar Arduino IDE

### Problema: "Erro ao fazer upload"
**Solução:**
- Pressionar botão "Boot" durante upload
- Verificar velocidade de baud
- Tentar outra porta USB

### Problema: "Sensor não responde"
**Solução:**
- Verificar pinagem
- Verificar tensão (deve ser 3.3V)
- Testar com código de exemplo do MAX30100

### Problema: "Blynk desconectando"
**Solução:**
- Verificar SSID e senha WiFi
- Verificar auth token
- Aumentar intervalo de keep-alive

## Testes Iniciais

### Teste 1: Leitura do Sensor
```cpp
void testSensor() {
  Serial.println("BPM: " + String(bpm));
  Serial.println("SpO2: " + String(spo2) + "%");
}
```

### Teste 2: Conectividade WiFi
```cpp
void testWiFi() {
  Serial.print("Status WiFi: ");
  Serial.println(WiFi.status());
}
```

### Teste 3: Blynk Connection
```cpp
void testBlynk() {
  Serial.println(Blynk.connected() ? "Conectado" : "Desconectado");
}
```

## Próximos Passos

1. ✅ Completar setup básico
2. 📊 Configurar banco de dados
3. 🔔 Implementar alertas
4. 📈 Adicionar gráficos históricos
5. 📱 Deploy app mobile

## Referências

- [Documentação ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [Blynk Documentation](https://docs.blynk.io/)
- [MAX30100 Datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf)

---

**Dúvidas?** Abra uma [issue](https://github.com/MateusRondon/Monitoramento-Cardiaco/issues)
