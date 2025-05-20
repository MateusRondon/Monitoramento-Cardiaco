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
    G -- Sim --> H[Zera BPM, SpO2 e tempo]
    G -- Não --> I[Marca dedo presente / Atualiza tempo]
    I --> J[Coleta Dados BPM  entre 60-100]
    J --> K[Coleta Dados SpO2  entre 90-100%]
    H --> L[Exibe no Serial Monitor]
    K --> L
    L --> M[Responde rota /bpm com JSON]
    
--
    ## 👨‍💻 Autor

    **Mateus Rondon**  
    Estudante de Engenharia da Computação no IFMT  
    🔗 [GitHub](https://github.com/MateusRondon) | 💻 Dev Full-Stack
