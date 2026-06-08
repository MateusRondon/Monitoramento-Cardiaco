
## 📊 Visão Geral da Organização (Ordem de Prioridade)

```
Monitoramento-Cardiaco/
│
├── 📄 README.md                    # ⭐ DOCUMENTAÇÃO PRINCIPAL - COMECE AQUI
├── 🗂️ STRUCTURE.md                 # ⭐ MAPA DE ESTRUTURA
├── 📝 .gitignore                   # Git configuração
├── 🔧 init.sh                      # Script de inicialização
│
├── 💻 src/                         # 🔴 CÓDIGO-FONTE (PRIORIDADE 1 - DEPLOY)
│   ├── 📄 README.md                # Descrição dos arquivos
│   ├── main.cpp                    # Arquivo principal ESP32
│   ├── sensor_reader.h             # Leitor do sensor MAX30100
│   ├── blynk_integration.h         # Integração com Blynk IoT
│   └── database.h                  # Gerenciamento de banco de dados
│
├── 🌐 Integracao-Web/              # 🟠 WEB (PRIORIDADE 2 - FRONTEND/BACKEND)
│   ├── 📄 README.md                # Documentação web
│   ├── backend/                    # Servidor Node.js/Express
│   │   ├── server.js               # Entry point backend
│   │   ├── config/
│   │   ├── routes/
│   │   ├── models/
│   │   ├── controllers/
│   │   ├── middleware/
│   │   └── package.json
│   ├── frontend/                   # Cliente React/TypeScript
│   │   ├── index.html
│   │   ├── public/
│   │   ├── src/
│   │   │   ├── components/
│   │   │   ├── pages/
│   │   │   ├── services/
│   │   │   ├── App.tsx
│   │   │   └── index.tsx
│   │   ├── package.json
│   │   └── tsconfig.json
│   ├── api_docs.md                 # Documentação REST API
│   ├── docker-compose.yml          # Orquestração containers
│   └── .env.example                # Template de variáveis
│
├── 📚 docs/                        # 🟡 DOCUMENTAÇÃO (PRIORIDADE 3)
│   ├── 📄 README.md                # Índice de documentação
│   ├── setup_guide.md              # Guia de instalação completa
│   ├── ARCHITECTURE.md             # Arquitetura do sistema
│   ├── API_reference.md            # Referência da API
│   ├── troubleshooting.md          # Solução de problemas
│   ├── CONTRIBUTING.md             # Guia para contribuidores
│   ├── deployment.md               # Guia de deploy
│   ├── security.md                 # Práticas de segurança
│   └── CHANGELOG.md                # Histórico de versões
│
├── 🧪 tests/                       # ✅ TESTES (PRIORIDADE 4)
│   ├── 📄 README.md                # Guia de testes
│   ├── sensor_tests.cpp            # Testes do sensor
│   ├── blynk_tests.cpp             # Testes Blynk
│   ├── database_tests.cpp          # Testes BD
│   ├── integration_tests.cpp       # Testes integração
│   ├── CMakeLists.txt              # Build config
│   └── fixtures/                   # Dados de teste
│
├── 🔌 hardware/                    # ⚙️ HARDWARE (PRIORIDADE 5)
│   ├── 📄 README.md                # Guia de hardware
│   ├── schematics/                 # Diagramas eletrônicos
│   │   └── MAX30100_ESP32.sch
│   ├── pcb/                        # Layout PCB
│   │   └── cardio_monitor.brd
│   ├── BOM.txt                     # Bill of Materials
│   ├── pinout.md                   # Pinagem completa
│   └── calibration.md              # Calibração de sensores
│
├── 🐍 python-test/                 # 🔬 TESTES PYTHON (SUPORTE)
│   ├── 📄 README.md                # Documentação Python
│   ├── sensor_simulator.py         # Simulador de sensor
│   ├── mqtt_tester.py              # Teste MQTT
│   ├── data_analyzer.py            # Análise de dados
│   ├── requirements.txt            # Dependências Python
│   └── config/                     # Configurações
│
├── 📋 Produto_Backlog/             # 📊 PLANEJAMENTO (DOCUMENTAÇÃO)
│   ├── 📄 README.md                # Índice do backlog
│   ├── user_stories.md             # Histórias de usuário
│   ├── requisitos.md               # Requisitos funcionais
│   ├── prototipo/                  # Protótipos e wireframes
│   └── mockups/                    # Mockups de UI
│
├── 📑 Artigo_Projeto/              # 📚 PESQUISA (DOCUMENTAÇÃO)
│   ├── 📄 README.md                # Índice de pesquisa
│   ├── referencias.md              # Referências bibliográficas
│   ├── especificacoes/             # Documentos técnicos
│   │   ├── ESP32_datasheet.pdf
│   │   ├── MAX30100_datasheet.pdf
│   │   └── Blynk_protocol.md
│   └── pesquisa/                   # Artigos científicos
│
├── .github/                        # ⚙️ CI/CD (GITHUB ACTIONS)
│   ├── 📄 workflows/README.md      # Documentação workflows
│   └── workflows/
│       ├── ci.yml                  # Pipeline testes
│       ├── deploy-staging.yml      # Deploy staging
│       ├── deploy-prod.yml         # Deploy produção
│       ├── lint.yml                # Linting
│       └── security-scan.yml       # Scan de segurança
│
└── .vscode/                        # 🖥️ IDE CONFIG
    ├── settings.json
    ├── extensions.json
    └── launch.json
```

---