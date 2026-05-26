# 🗂️ ESTRUTURA REORGANIZADA DO PROJETO

## 📊 Visão Geral da Organização

```
Monitoramento-Cardiaco/
│
├── 📄 README.md                    # Documentação principal
├── 🗂️ STRUCTURE.md                 # Este mapa de estrutura
├── 📝 .gitignore                   # Git ignore rules
├── 🔧 init.sh                      # Script de inicialização
│
├── 💻 src/                         # 🔴 CÓDIGO-FONTE (Core do projeto)
│   ├── 📄 README.md                # Descrição dos arquivos
│   ├── main.cpp                    # Arquivo principal
│   ├── sensor_reader.h             # Leitor do sensor MAX30100
│   ├── blynk_integration.h         # Integração Blynk
│   └── database.h                  # Gerenciamento BD
│
├── 🔌 hardware/                    # ⚙️ HARDWARE (Esquemas e PCB)
│   ├── 📄 README.md                # Guia de hardware
│   ├── schematics/                 # Diagramas esquemáticos
│   │   └── MAX30100_ESP32.sch
│   ├── pcb/                        # Layout PCB
│   │   └── cardio_monitor.brd
│   └── BOM.txt                     # Lista de componentes
│
├── 📚 docs/                        # 📖 DOCUMENTAÇÃO (Guias e referencias)
│   ├── 📄 README.md                # Índice de documentação
│   ├── setup_guide.md              # Guia de instalação
│   ├── API_reference.md            # Referência de API
│   ├── troubleshooting.md          # Solução de problemas
│   ├── ARCHITECTURE.md             # Arquitetura do sistema
│   └── CONTRIBUTING.md             # Guia para contribuidores
│
├── 🧪 tests/                       # ✅ TESTES (Testes unitários)
│   ├── 📄 README.md                # Guia de testes
│   ├── sensor_tests.cpp            # Testes do sensor
│   ├── blynk_tests.cpp             # Testes Blynk
│   └── database_tests.cpp          # Testes BD
│
├── 🐍 python-test/                 # 🔬 TESTES PYTHON (Scripts auxiliares)
│   ├── 📄 README.md                # Documentação Python
│   ├── sensor_simulator.py         # Simulador de sensor
│   ├── mqtt_tester.py              # Teste MQTT
│   ├── data_analyzer.py            # Análise de dados
│   └── requirements.txt            # Dependências
│
├── .github/                        # ⚙️ CI/CD (GitHub Actions)
│   ├── 📄 workflows/README.md      # Documentação workflows
│   └── workflows/
│       ├── ci.yml                  # Pipeline de CI/CD
│       ├── tests.yml               # Execução de testes
│       └── deploy.yml              # Deploy automático
│
├── .vscode/                        # 🖥️ IDE CONFIG (VS Code)
│   └── settings.json
│
├── 📋 Artigo_Projeto/              # 📑 PESQUISA (Artigos e referências)
│   ├── 📄 README.md                # Índice de pesquisa
│   ├── referencias.md              # Lista de referências
│   ├── especificacoes/             # Documentos técnicos
│   └── pesquisa/                   # Artigos científicos
│
├── 📊 Produto_Backlog/             # 📋 PLANEJAMENTO (Backlog)
│   ├── 📄 README.md                # Índice do backlog
│   ├── user_stories.md             # Histórias de usuário
│   ├── requisitos.md               # Requisitos funcionais
│   ├── prototipo/                  # Protótipos
│   └── mockups/                    # Mockups de UI
│
└── 🌐 Integracao-Web/              # 🔗 WEB (Frontend/Backend)
    ├── 📄 README.md                # Documentação web
    ├── backend/                    # Servidor
    │   ├── server.js
    │   ├── routes/
    │   └── models/
    ├── frontend/                   # Cliente
    │   ├── index.html
    │   ├── css/
    │   ├── js/
    │   └── components/
    └── api_docs.md                 # Documentação API
```

## 🎯 Categorização por Tipo

### 🔴 DESENVOLVIMENTO (Código)
```
src/                    ← Código Arduino/C++
tests/                  ← Testes C++
python-test/            ← Testes Python
.github/workflows/      ← CI/CD
```

### 📖 DOCUMENTAÇÃO
```
docs/                   ← Guias e referências
Artigo_Projeto/         ← Pesquisa acadêmica
Produto_Backlog/        ← Planejamento
```

### ⚙️ CONFIGURAÇÃO
```
.gitignore              ← Git config
.vscode/                ← IDE config
init.sh                 ← Setup script
```

### 🔗 INTEGRAÇÃO
```
Integracao-Web/         ← Web
hardware/               ← Hardware
```

## 📍 Mapa de Localização

| O que procuro | Onde está |
|---------------|-----------|
| **Código ESP32** | `src/` |
| **Esquema eletrônico** | `hardware/schematics/` |
| **Como instalar** | `docs/setup_guide.md` |
| **Testes** | `tests/` ou `python-test/` |
| **Histórias de usuário** | `Produto_Backlog/user_stories.md` |
| **Dashboard web** | `Integracao-Web/frontend/` |
| **Servidor backend** | `Integracao-Web/backend/` |
| **Solução de problemas** | `docs/troubleshooting.md` |

## 🏗️ Arquitetura em Camadas

```
┌─────────────────────────────────────┐
│         🌐 Frontend (Web)           │ ← Integracao-Web/frontend/
├─────────────────────────────────────┤
│         🔗 Backend (API)            │ ← Integracao-Web/backend/
├─────────────────────────────────────┤
│      ☁️ Blynk IoT Platform          │ ← Integração externa
├─────────────────────────────────────┤
│  💾 Database (Firebase/MySQL)       │ ← Backend
├─────────────────────────────────────┤
│    📡 ESP32 Microcontroller         │ ← src/
├─────────────────────────────────────┤
│      🔌 MAX30100 Sensor             │ ← hardware/
└─────────────────────────────────────┘
```

## 🚀 Fluxo de Desenvolvimento

```
1. PLANEJAMENTO
   Produto_Backlog/ → Definir requisitos
        ↓
2. PESQUISA
   Artigo_Projeto/ → Estudar tecnologias
        ↓
3. DESENVOLVIMENTO
   src/ → Escrever código
        ↓
4. TESTES
   tests/ → Validar funcionamento
        ↓
5. DOCUMENTAÇÃO
   docs/ → Documentar
        ↓
6. INTEGRAÇÃO
   Integracao-Web/ → Conectar sistemas
        ↓
7. CI/CD
   .github/workflows/ → Deploy automático
```

## 📝 Convenções de Nomenclatura

### Pastas
- **Principais:** PascalCase ou snake_case
  - ✅ `Produto_Backlog/` 
  - ✅ `Integracao-Web/`
  - ✅ `docs/`

- **Ocultas:** _snake_case (começam com .)
  - `.github/`
  - `.vscode/`

### Arquivos
- **Código:** snake_case.ext
  - ✅ `sensor_reader.h`
  - ✅ `blynk_integration.h`

- **Docs:** snake_case.md
  - ✅ `setup_guide.md`
  - ✅ `API_reference.md`

- **Config:** como está
  - ✅ `init.sh`
  - ✅ `.gitignore`

## 🎨 Emojis Utilizados

| Emoji | Significado |
|-------|------------|
| 📄 | Arquivo de documentação |
| 🗂️ | Estrutura/Organização |
| 💻 | Código/Desenvolvimento |
| 🔌 | Hardware |
| 📚 | Documentação |
| 🧪 | Testes |
| 🐍 | Python |
| ⚙️ | Configuração/CI-CD |
| 🌐 | Web/Internet |
| 📋 | Planejamento/Backlog |
| 📑 | Pesquisa/Artigos |

## ✅ Checklist de Organização

- ✅ Cada pasta principal tem `README.md`
- ✅ Código separado de documentação
- ✅ Testes em pasta dedicada
- ✅ CI/CD configurado
- ✅ Hardware documentado
- ✅ Web integrada
- ✅ Convenções de nome seguidas
- ✅ Emojis para identidade visual

## 🔗 Links Úteis

- Entender código → `src/README.md`
- Configurar hardware → `hardware/README.md` + `docs/setup_guide.md`
- Rodar testes → `tests/README.md`
- Deploy web → `Integracao-Web/README.md`
- Troubleshooting → `docs/troubleshooting.md`

---

**Status:** ✅ Estrutura Organizacional Completa
**Última atualização:** Maio 2026
**Versão:** 1.0
