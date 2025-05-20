# Monitoramento-Cardiaco
# Monitoramento de Batimentos e Oxigenação com ESP32

```mermaid
flowchart TD
    A[Início do Setup] --> B[Inicializa Serial e Wi-Fi]
    B --> C[Conecta ao Wi-Fi]
    C --> D[Inicia o Servidor Web]
    D --> E[Define Rotas HTTP: /, /bpm, etc]
    E --> F[Loop Principal]

    F --> G{Leitura do Sensor < 50?}
    G -- Sim --> H[Zera BPM, SpO2 e tempo]
    G -- Não --> I[Marca dedo presente / Atualiza tempo]
    I --> J[Coleta Dados BPM  entre 60-100]
    J --> K[Coleta Dados SpO2  entre 90-100%]
    H --> L[Exibe no Serial Monitor]
    K --> L
    L --> M[Responde rota /bpm com JSON]

    click A href "#" "Setup"
    click F href "#" "Loop"
    click M href "#" "/bpm"






# 🗂️ Product Backlog - Projeto ESP32 Monitor de Saúde

## Épico 1: Configuração e Preparação do Ambiente

| ID  | User Story                                                                                 | Status       | Responsável     | Tamanho | Estimativa |
|-----|---------------------------------------------------------------------------------------------|--------------|------------------|---------|------------|
| 1   | Como desenvolvedor, preciso configurar o ambiente para desenvolvimento no ESP32             | 🟡 In progress | MateusRondon     | XS      | 1          |
| 2   | Como desenvolvedor, quero garantir a conexão Wi-Fi e servidor web funcionando               | 🟡 In progress | MateusRondon     | XS      | 1          |

## Épico 2: Coleta e Visualização de Dados

| ID  | User Story                                                                                 | Status  | Responsável     | Tamanho | Estimativa |
|-----|---------------------------------------------------------------------------------------------|---------|------------------|---------|------------|
| 4   | Como usuário, preciso que o sensor MAX30100 me forneça dados de batimentos cardíacos       | ✅ Done  | MateusRondon     | L       | 2          |
| 5   | Como dev, preciso visualizar os dados localmente para garantir que o sensor está funcionando | ✅ Done  | MateusRondon     | L       | 2          |
| 6   | Como dev, quero integrar o ESP32 ao Blynk para envio remoto de dados                        | ✅ Done  | MateusRondon     | L       | 2          |
| 7   | Como usuário, quero ver meus batimentos e oxigenação no painel web                         | ✅ Done  | MateusRondon     | L       | 2          |
| 8   | Como dev, desejo que os dados sejam enviados via MQTT                                      | ✅ Done  | MateusRondon     | L       | 2          |
| 9   | Como usuário, desejo ser notificado quando o batimento sair da faixa saudável              | 🟡 In progress | MateusRondon     | L       | 2          |
| 10  | [Defina uma nova história aqui]                                                             | ⏳ Todo | MateusRondon     | L       | 2          |
