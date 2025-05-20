# 💓 Monitoramento Cardíaco com ESP32

Projeto de IoT que realiza a **leitura de batimentos cardíacos (BPM)** e **oxigenação sanguínea (SpO2)** utilizando um ESP32 com sensor MAX30100/30102, exibindo os dados via **interface web responsiva**.

---

## 🛠 Tecnologias Utilizadas

- ⚙️ **ESP32**
- 📡 Wi-Fi embutido
- 📊 Sensor de Pulso MAX30100/30102 ( Coletando dados neste protótipo)
- 🌐 Interface Web com HTML, CSS e JavaScript
- 📦 Servidor Web interno via `WebServer.h`

---

## 🔁 Fluxo de Funcionamento

```mermaid
flowchart TD
  
    A[Início do Setup] --> B[Inicializa Serial e Wi-Fi]
    B --> C[Conecta ao Wi-Fi]
    C --> D[Inicia o Servidor Web]
    D --> E[Define Rotas HTTP: /, /bpm, etc]
    E --> F[Loop Principal]

    F --> G{Leitura do Sensor < 50?}
    G -- Não --> H[Zera BPM, SpO2 e tempo]
    G -- Sim --> I[Marca dedo presente / Atualiza tempo]
    I --> J{Dedo presente?}
    J -- Não --> I[Loop até encontrar dedo]
    J -- Sim --> K[Coleta Dados BPM entre 60-100]
    K --> L[Coleta Dados SpO2 entre 90-100%]
    H --> M[Exibe no Serial Monitor]
    L --> M
    M --> N[Responde rota /bpm com JSON]
    M --> O[Responde rota /spo2 com JSON]
    M --> P[Exibe dados na interface Web site]
    P --> Q[Envia dados para o Firebase]
    Q --> R[Confirmação de envio no console]


# Fluxograma: ESP32 + MAX30100 -> Envio de Dados para Site HTML

Este fluxograma descreve o processo de conexão entre um **ESP32** e o sensor **MAX30100** para coletar dados de batimentos cardíacos e enviar esses dados para um site HTML gerado pelo próprio ESP32.

## Passos:

1. **Inicialização do ESP32 e MAX30100**:
   - O ESP32 é inicializado.
   - O sensor MAX30100 é conectado e configurado.
   - O ESP32 é configurado para atuar como um servidor web.

2. **Leitura de Dados do Sensor MAX30100**:
   - O ESP32 coleta dados do sensor MAX30100 (batimentos cardíacos).

3. **Processamento dos Dados**:
   - O ESP32 processa os dados recebidos, por exemplo, obtendo a taxa de batimentos cardíacos.

4. **Geração da Página HTML**:
   - O ESP32 gera uma página HTML dinâmica que exibe os dados de batimentos cardíacos.

5. **Configuração do Servidor Web no ESP32**:
   - O ESP32 é configurado como servidor web local.
   - A página HTML é hospedada e fica acessível através do IP do ESP32.

6. **Exibição dos Dados no Navegador**:
   - O usuário acessa o servidor web do ESP32 através de um navegador.
   - Os dados de batimentos cardíacos são exibidos em tempo real na página HTML.

7. **Atualização Contínua dos Dados**:
   - A cada nova leitura do sensor, os dados na página HTML são atualizados em tempo real.

---

## Fluxograma

```plaintext
+-----------------------------------+
|           Inicialização           |
|        do ESP32 e MAX30100        |
+-----------------------------------+
                |
                v
+-----------------------------------+
| Coleta de Dados do Sensor        |
|    MAX30100 (batimentos)         |
+-----------------------------------+
                |
                v
+-----------------------------------+
| Processamento de Dados (bat.)    |
|     (ex: média, controle)        |
+-----------------------------------+
                |
                v
+-----------------------------------+
|   Geração de Página HTML Dinâmica|
| (com dados de batimentos cardíacos)|
+-----------------------------------+
                |
                v
+-----------------------------------+
|    Início do Servidor Web no ESP32|
+-----------------------------------+
                |
                v
+-----------------------------------+
|   Acesso ao Site via Navegador   |
|   (Endereço IP do ESP32)         |
+-----------------------------------+
                |
                v
+-----------------------------------+
|   Exibição em Tempo Real         |
|  dos Dados de Batimentos Cardíacos|
+-----------------------------------+
                |
                v
+-----------------------------------+
|   Atualização Contínua dos Dados |
|    na Página HTML                |
+-----------------------------------+
