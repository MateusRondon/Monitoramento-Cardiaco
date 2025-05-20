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
