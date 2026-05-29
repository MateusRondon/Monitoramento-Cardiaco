# 📋 Estrutura Reorganizada - Monitoramento Cardíaco

## 🗂️ Organização do Repositório

Este documento descreve a estrutura profissional e otimizada do projeto para facilitar navegação, desenvolvimento e deploy.

---

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

## 🎯 Categorização por Tipo

### 🔴 DESENVOLVIMENTO (Código)
```
src/                    ← Código Arduino/C++ (CRÍTICO)
Integracao-Web/         ← Frontend + Backend (CRÍTICO)
tests/                  ← Testes C++ (IMPORTANTE)
python-test/            ← Testes Python (IMPORTANTE)
.github/workflows/      ← CI/CD (IMPORTANTE)
```

### 🟡 DOCUMENTAÇÃO
```
docs/                   ← Guias técnicas (IMPORTANTE)
Artigo_Projeto/         ← Pesquisa acadêmica (SUPORTE)
Produto_Backlog/        ← Planejamento (SUPORTE)
README.md               ← Entry point (CRÍTICO)
```

### ⚙️ CONFIGURAÇÃO
```
.gitignore              ← Git config
.vscode/                ← IDE config
init.sh                 ← Setup script
.env.example            ← Template variáveis
```

### 🔗 HARDWARE
```
hardware/               ← Schematics e PCB (IMPORTANTE)
```

---

## 📍 Mapa Rápido de Localização

| O que procuro | Onde está | Prioridade |
|---------------|-----------|-----------|
| **Comece aqui** | `README.md` | ⭐⭐⭐ |
| **Estrutura do projeto** | `STRUCTURE.md` | ⭐⭐⭐ |
| **Código ESP32** | `src/main.cpp` | ⭐⭐⭐ |
| **Dashboard Web** | `Integracao-Web/frontend/` | ⭐⭐⭐ |
| **API Backend** | `Integracao-Web/backend/` | ⭐⭐⭐ |
| **Como instalar** | `docs/setup_guide.md` | ⭐⭐ |
| **Arquitetura do sistema** | `docs/ARCHITECTURE.md` | ⭐⭐ |
| **Guia de deploy** | `docs/deployment.md` | ⭐⭐ |
| **Esquema eletrônico** | `hardware/schematics/` | ⭐⭐ |
| **Executar testes** | `tests/` ou `python-test/` | ⭐⭐ |
| **Solução de problemas** | `docs/troubleshooting.md` | ⭐ |
| **Histórias de usuário** | `Produto_Backlog/user_stories.md` | ⭐ |
| **Referências** | `Artigo_Projeto/referencias.md` | ⭐ |

---

## 🏗️ Arquitetura em Camadas

```
┌──────────────────────────────────────────────┐
│         🌐 Frontend (React/TypeScript)       │ ← Integracao-Web/frontend/
├──────────────────────────────────────────────┤
│         🔗 Backend API (Node.js/Express)     │ ← Integracao-Web/backend/
├──────────────────────────────────────────────┤
│    ☁️ Blynk IoT Platform (Integração)        │ ← Configuração externa
├──────────────────────────────────────────────┤
│  💾 Database (Firebase/MySQL)                │ ← Backend config
├──────────────────────────────────────────────┤
│    📡 ESP32 Microcontroller (C++)            │ ← src/main.cpp
├──────────────────────────────────────────────┤
│  🔌 MAX30100 Sensor (Hardware)               │ ← hardware/
└──────────────────────────────────────────────┘
```

---

## 🚀 Fluxo de Desenvolvimento & Deployment

### Ciclo de Desenvolvimento

```
1. PLANEJAMENTO
   Produto_Backlog/ → Definir requisitos
        ↓
2. PESQUISA & ESPECIFICAÇÃO
   Artigo_Projeto/ → Estudar tecnologias
   docs/ARCHITECTURE.md → Projetar arquitetura
        ↓
3. DESENVOLVIMENTO
   src/ → Código ESP32
   Integracao-Web/ → Frontend + Backend
        ↓
4. TESTES
   tests/ → Testes unitários
   python-test/ → Testes de integração
        ↓
5. DOCUMENTAÇÃO
   docs/ → Documenta mudanças
        ↓
6. CI/CD & VALIDAÇÃO
   .github/workflows/ → Testes automáticos
        ↓
7. DEPLOY
   docs/deployment.md → Deploy produção
```

### Processo de Deploy

```
LOCAL DEVELOPMENT
    ↓
COMMIT & PUSH (main branch)
    ↓
GITHUB ACTIONS (CI/CD)
    ├─ Lint & Format Check
    ├─ Unit Tests
    ├─ Build Firmware
    ├─ Security Scan
    └─ Build Docker Images
    ↓
STAGING ENVIRONMENT
    ├─ Deploy Backend
    ├─ Deploy Frontend
    ├─ Run Integration Tests
    └─ Manual Testing
    ↓
PRODUCTION DEPLOYMENT
    ├─ Tag Release
    ├─ Deploy Backend
    ├─ Deploy Frontend
    ├─ Update Firmware on Devices
    └─ Monitor & Log
```

---

## 📝 Convenções de Nomenclatura

### Pastas Principais

- **PascalCase com Hífen:**
  - ✅ `Integracao-Web/`
  - ✅ `Artigo_Projeto/`
  - ✅ `Produto_Backlog/`

- **snake_case:**
  - ✅ `src/`
  - ✅ `docs/`
  - ✅ `tests/`
  - ✅ `python-test/`

- **Ocultas (começam com .):**
  - `.github/`
  - `.vscode/`
  - `.gitignore`

### Arquivos de Código

- **snake_case.ext:**
  - ✅ `sensor_reader.h`
  - ✅ `blynk_integration.h`
  - ✅ `database_tests.cpp`

### Documentos

- **snake_case.md:**
  - ✅ `setup_guide.md`
  - ✅ `API_reference.md`
  - ✅ `troubleshooting.md`

- **UPPERCASE.md (especiais):**
  - ✅ `README.md`
  - ✅ `STRUCTURE.md`
  - ✅ `ARCHITECTURE.md`
  - ✅ `CONTRIBUTING.md`
  - ✅ `CHANGELOG.md`

### Configuração

- **Como está:**
  - ✅ `init.sh`
  - ✅ `.gitignore`
  - ✅ `CMakeLists.txt`
  - ✅ `docker-compose.yml`

---

## 🎨 Emojis Padrão

| Emoji | Significado | Uso |
|-------|------------|-----|
| 📄 | Documentação | Headers de docs |
| 🗂️ | Estrutura/Organização | Folders e índices |
| 💻 | Código/Desenvolvimento | Pastas de código |
| 🔌 | Hardware | Hardware configs |
| 📚 | Documentação | Pasta docs |
| 🧪 | Testes | Pasta tests |
| 🐍 | Python | Scripts Python |
| ⚙️ | Configuração/CI-CD | Config files |
| 🌐 | Web/Internet | Web apps |
| 📋 | Planejamento/Backlog | Planejamento |
| 📑 | Pesquisa/Artigos | Pesquisa |
| 🚀 | Deploy/Produção | Deploy info |
| 📊 | Dashboard/Analytics | Analytics |
| 🔐 | Segurança | Security |

---

## ✅ Checklist de Organização

- ✅ Cada pasta principal tem `README.md`
- ✅ Código separado de documentação
- ✅ Testes em pastas dedicadas
- ✅ CI/CD configurado e automatizado
- ✅ Hardware documentado com schematics
- ✅ Web (frontend + backend) integrada
- ✅ Convenções de nome consistentes
- ✅ Emojis para identidade visual
- ✅ Prioridades claras para deploy
- ✅ Documentação de deployment
- ✅ Guias de troubleshooting
- ✅ Configurações de segurança

---

## 🔗 Links Rápidos de Referência

### Começar
- ⭐ **Primeiro:** [`README.md`](README.md) - Visão geral e Quick Start
- ⭐ **Segundo:** [`STRUCTURE.md`](STRUCTURE.md) - Este arquivo

### Desenvolver
- 💻 **Código:** [`src/README.md`](src/README.md) ou `src/main.cpp`
- 🌐 **Web:** [`Integracao-Web/README.md`](Integracao-Web/README.md)
- 🧪 **Testes:** [`tests/README.md`](tests/README.md)

### Documentação Técnica
- 🏗️ **Arquitetura:** [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
- 📡 **API:** [`Integracao-Web/api_docs.md`](Integracao-Web/api_docs.md)
- 🔐 **Segurança:** [`docs/security.md`](docs/security.md)

### Setup & Deploy
- ⚙️ **Instalação:** [`docs/setup_guide.md`](docs/setup_guide.md)
- 🚀 **Deploy:** [`docs/deployment.md`](docs/deployment.md)
- 🆘 **Problemas:** [`docs/troubleshooting.md`](docs/troubleshooting.md)

### Hardware
- 🔌 **Hardware:** [`hardware/README.md`](hardware/README.md)
- 📋 **Componentes:** [`hardware/BOM.txt`](hardware/BOM.txt)

### Planejamento
- 📊 **Backlog:** [`Produto_Backlog/README.md`](Produto_Backlog/README.md)
- 📚 **Referências:** [`Artigo_Projeto/referencias.md`](Artigo_Projeto/referencias.md)

---

## 📈 Estatísticas do Projeto

```
Total de Pastas:        12 principais + subpastas
Total de Documentos:    25+ arquivos .md
Linhas de Código:       ~5000+ (estimado)
Cobertura de Testes:    85%+ (target)
Linguagens:             C++, TypeScript, Python, Shell
Frameworks:             Arduino, Express, React, Blynk
```

---

## 🔄 Atualizações Recentes

### Maio 2026

- ✅ Reorganização completa da estrutura
- ✅ Prioridades claras para deploy
- ✅ Documentação aprimorada
- ✅ Templates para `.env` e config
- ✅ CI/CD workflows completos
- ✅ Guia de deployment adicionado
- ✅ Convenções padronizadas

---

## 📞 Contato & Suporte

**GitHub Issues:** [Abra uma issue](https://github.com/MateusRondon/Monitoramento-Cardiaco/issues)

**Email:** mateus.rondon@email.com

**Documentação:** Consulte [`docs/troubleshooting.md`](docs/troubleshooting.md)

---

<div align="center">

**Última atualização:** 29 Maio 2026  
**Versão:** 2.0  
**Status:** ✅ Estrutura Profissional Completa

**[← Voltar para README.md](README.md)**

</div>
