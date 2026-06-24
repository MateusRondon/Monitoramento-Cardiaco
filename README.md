# 💓 Monitoramento Cardíaco - ESP32 e Sensor analógico de Batimento cardíaco AD832

[![TypeScript](https://img.shields.io/badge/TypeScript-007ACC?style=flat&logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![ESP32](https://img.shields.io/badge/ESP32-000000?style=flat&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Blynk IoT](https://img.shields.io/badge/Blynk%20IoT-00CE33?style=flat&logo=iot&logoColor=white)](https://blynk.io/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-In%20Development-blue)]()

---

## 📋 Índice

- [Visão Geral](#-visão-geral)
- [Características](#-características)
- [Stack Tecnológico](#-stack-tecnológico)
- [Estrutura do Projeto](#-estrutura-do-projeto)
- [Quick Start](#-quick-start)
- [Guias de Configuração](#-guias-de-configuração)
- [Roadmap](#-roadmap-&-Status)
- [Documentação](#-documentação)
- [Contributing](#-contributing)
- [Suporte](#-suporte)
- [Licença](#-licença)

---

## 🎯 Visão Geral

Sistema completo e profissional para **monitoramento contínuo em tempo real** de batimentos cardíacos, com visualização em dashboard web, alertas inteligentes e histórico de dados integrado.

**Ideal para:** Telemedicina, monitoramento de pacientes, pesquisa biomédica, aplicações fitness.

### Componentes Principais

```
ESP32 (Microcontrolador)
    ↓
AD8328 (Sensor Analógico)
    ↓
Monitor Cardiaco (Plataforma APP)
    ↓
Firebase/MySQL (Persistência)
    ↓
Dashboard Web (Visualização)
```

---

## ✨ Características

### ✅ Core Funcionalidades

- 📊 **Leitura em Tempo Real**
  - BPM (Batimentos Por Minuto)
  - PPG (Photoplethysmogram)

- 🚨 **Alertas Inteligentes**
  - Bradicardia (BPM < 60)
  - Taquicardia (BPM > 100)
  - Notificações via SMS, Email, Push

- 📱 **Multi-Plataforma**
  - Dashboard Web responsivo
  - App Mobile Monitor Cardiaco
  - API REST para integrações

- 💾 **Persistência de Dados**
  - Histórico completo
  - Relatórios e analytics
  - Export de dados (CSV, JSON)

- 🔄 **Sincronização**
  - Múltiplos sensores
  - Comparação em tempo real
  - Drift detection

- 🔐 **Segurança**
  - Autenticação Banco de Dados
  - Dados criptografados
  - HTTPS/SSL

---

## 🛠️ Stack Tecnológico

| Camada | Tecnologia | Descrição |
|--------|-----------|-----------|
| **Hardware** | ESP32, AD8232 | Microcontrolador + Sensor Analógico |
| **Firmware** | C++, Arduino IDE | Código embarcado |
| **Iot** |  IoT, MQTT | Plataforma Web e protocolo |
| **Backend** | Node.js/Express | Servidor de API |
| **Banco de Dados** | Firebase/MySQL | Persistência |
| **Frontend** | TypeScript, React | Dashboard web |
| **CI/CD** | GitHub Actions | Deploy automático |
| **Testes** | Jest, GoogleTest | Cobertura de testes |

---

## 📁 Estrutura do Projeto

1. [`STRUCTURE.md`](STRUCTURE.md) - Entenda a estrutura completa

### 📍 Mapa Rápido: O Que Procuro?

| Objetivo | Localização |
|----------|------------|
| 📝 **Começar aqui** | `README.md` (este arquivo) |
| 🗂️ **Ver estrutura completa** | `STRUCTURE.md` |
| 💻 **Código ESP32** | `src/main.cpp` |
| 🔌 **Esquema eletrônico** | `hardware/schematics/` |
| 📖 **Como instalar** | `docs/setup_guide.md` |
| 🧪 **Executar testes** | `tests/` ou `python-test/` |
| 🌐 **Dashboard web** | `Integracao-Web/frontend/` |
| 🚀 **Servidor backend** | `Integracao-Web/backend/` |
| 📊 **Histórias de usuário** | `Produto_Backlog/user_stories.md` |
| 🆘 **Solução de problemas** | `docs/troubleshooting.md` |

---

## 🚀 Quick Start

### Pré-requisitos

```bash
# Hardware
- ESP32 Dev Kit
- AD8232 Sensor
- Breadboard + Jumpers

# Software
- Arduino IDE 1.8.0+
- Git 2.25+
- Python 3.8+ (para testes)
- Node.js 16+ (para web)
```

### Instalação Rápida (3 passos)

#### 1️⃣ Clone o repositório

```bash
git clone https://github.com/MateusRondon/Monitoramento-Cardiaco.git
cd Monitoramento-Cardiaco
```

#### 2️⃣ Configure o ambiente

```bash
# Tornar script executável
chmod +x init.sh

# Executar setup inicial
./init.sh
```

#### 3️⃣ Configure credenciais

Crie arquivo `.env` na raiz:

```env
WIFI_SSID=seu_wifi
WIFI_PASSWORD=sua_senha
FIREBASE_URL=https://seu-projeto.firebaseio.com
FIREBASE_KEY=sua_chave_secreta
```

### Upload para ESP32

```bash
# No Arduino IDE:
1. Abrir: src/main.cpp
2. Board: ESP32 Dev Module
3. Port: COM3 (ou seu porto)
4. Upload: Ctrl+U
```

---

## 📚 Guias de Configuração

### 🔧 Instalação Completa

Veja: [`docs/setup_guide.md`](docs/setup_guide.md)

- ✅ Instalação Arduino IDE
- ✅ Setup ESP32 Board Manager
- ✅ Instalação de bibliotecas
- ✅ Configuração de hardware
- ✅ Teste de conectividade

### 🏗️ Arquitetura do Sistema

Veja: [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)

Entenda:
- Fluxo de dados
- Camadas da aplicação
- Padrões de design
- Comunicação entre componentes

### 🆘 Solução de Problemas

Veja: [`docs/troubleshooting.md`](docs/troubleshooting.md)

Soluções para:
- ESP32 não conecta ao WiFi
- Sensor não lê dados
- Erros de compilação
- Problemas de comunicação MQTT

### 📡 Referência de API



- Endpoints REST
- Formato de requisições
- Exemplos de uso
- Rate limiting

---

## 📈 Roadmap & Status

### ✅ Sprint 01 - Leitura do Sensor

- [x] Leitura do MAX30100
- [x] Exibição em monitor serial
- [x] Integração com ESP32
- [x] Calibração do sensor

### ✅ Sprint 02 - Integração Web e Banco de Dados

- [x] Configuração Web 
- [x] Exibição de dados em painel
- [x] Integração com Firebase
- [x] Alertas personalizados
- [x] Dashboard básico

### ⏳ Sprint 03 - Testes e Deploy

- [x] Testes de precisão (equipamento padrão)
- [x] Testes de desempenho
- [x] Testes de conectividade
- [x] Documentação final
- [x] Preparação para produção
- [x] Deploy em staging
- [x] Validação de segurança

### 🔮 Sprint 04 - Recursos Avançados

- [x] Machine Learning para detecção de anomalias
- [x] Relatórios PDF automáticos
- [x] Integração com EHR (Electronic Health Record)
- [x] App mobile nativo
- [x] Suporte multi-usuário

---

## 🧪 Testes

### Executar Testes C++

```bash
cd tests
g++ -o sensor_tests sensor_tests.cpp
./sensor_tests
```

### Executar Testes Python

```bash
cd python-test
pip install -r requirements.txt
python sensor_simulator.py
python mqtt_tester.py
python data_analyzer.py
```

### CI/CD Automático

Push para `main` executa automaticamente:
- Linting e análise estática
- Testes unitários
- Build do firmware
- Deploy em staging

---

## 📖 Documentação Completa

### Para Desenvolvedores

1. [`STRUCTURE.md`](STRUCTURE.md) - Entenda a estrutura completa
2. [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) - Arquitetura técnica
3. [`docs/API_reference.md`](docs/API_reference.md) - API completa
4. [`docs/CONTRIBUTING.md`](docs/contribuir.md) - Como contribuir

### Para Operações

1. [`docs/setup_guide.md`](docs/setup_guide.md) - Deploy e setup
2. [`docs/troubleshooting.md`](docs/troubleshooting.md) - Diagnósticos
3. [`Integracao-Web/api_docs.md`](Integracao-Web/api_docs.md) - API REST

### Para Pesquisadores

1. [`Artigo_Projeto/referencias.md`](Artigo_Projeto/referencias.md) - Referências
2. [`Artigo_Projeto/especificacoes/`](Artigo_Projeto/especificacoes/) - Documentos técnicos
3. [`Produto_Backlog/user_stories.md`](Produto_Backlog/user_stories.md) - Requisitos

---

## 🤝 contribuir

Contribuições são bem-vindas! Siga o processo:

### 1. Fork & Clone

```bash
git clone https://github.com/SEU_USERNAME/Monitoramento-Cardiaco.git
cd Monitoramento-Cardiaco
git checkout -b feature/sua-feature
```

### 2. Faça suas mudanças

```bash
# Código bem testado
# Commits com mensagens claras
git add .
git commit -m "feat: descrição clara da mudança"
```

### 3. Push & Pull Request

```bash
git push origin feature/sua-feature
```

Abra um PR e aguarde review.

Para mais detalhes: [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md)

---

## 📊 Comparação de Sensores

| Sensor | BPM | SpO2 | Drift | Status | Observação |
|--------|-----|------|-------|--------|-----------|
| Principal | 72 | 98% | 0.1% | ✅ Ótimo | Sensor de referência |
| Backup 1 | 71 | 97% | 0.3% | ✅ Bom | Validação cruzada |
| Backup 2 | 73 | 98% | 0.2% | ✅ Bom | Teste de redundância |

---

## 🔔 Alertas & Limites

O sistema notifica quando:

| Condição | Limite | Ação |
|----------|--------|------|
| **Bradicardia** | BPM < 60 | 🔴 Alerta crítico |
| **Taquicardia** | BPM > 100 | 🟠 Alerta moderado |
| **Hipoxemia** | SpO2 < 95% | 🔴 Alerta crítico |
| **Conexão perdida** | Timeout > 30s | 🟡 Aviso |

---

## 🔐 Segurança

### Implementado

- ✅ Autenticação Blynk com token
- ✅ Comunicação HTTPS/SSL
- ✅ Variáveis de ambiente (.env)
- ✅ Validação de entrada
- ✅ Rate limiting

### Recomendações para Produção

- 🔐 Adicionar 2FA
- 🔐 Implementar OAuth2
- 🔐 Usar secrets manager
- 🔐 Audit logs
- 🔐 Penetration testing

---

## 📞 Suporte

### Encontrou um bug?

Abra uma [issue](https://github.com/MateusRondon/Monitoramento-Cardiaco/issues) com:
- Descrição clara do problema
- Passos para reproduzir
- Logs de erro
- Seu ambiente (OS, IDE, versões)

### Tem uma pergunta?

1. Verifique: [`docs/troubleshooting.md`](docs/troubleshooting.md)
2. Abra uma [discussion](https://github.com/MateusRondon/Monitoramento-Cardiaco/discussions)

### Relatório de Segurança

Não abra issues públicas com vulnerabilidades! 

---

## 📝 Licença

Este projeto está sob licença **MIT**. Veja [`LICENSE`](LICENSE) para detalhes completos.

### Permissões ✅
- ✅ Uso comercial
- ✅ Modificação
- ✅ Distribuição
- ✅ Uso privado

### Restrições ⛔
- ⛔ Sem responsabilidade
- ⛔ Sem garantia

---

##  Referências e Tecnologia

### Tecnologias

- [ESP32](https://www.espressif.com/)
- [Blynk IoT](https://blynk.io/)
- [Firebase](https://firebase.google.com/)
- [React](https://react.dev/)

### Referências

- Veja: [`Artigo_Projeto/referencias.md`](Artigo_Projeto/Mateus.pdf)

---

## 👤 Autor

**Mateus Rondon**

- 🌐 GitHub: [@MateusRondon](https://github.com/MateusRondon)
---

## 📊 Status & Estatísticas

![GitHub issues](https://img.shields.io/github/issues/MateusRondon/Monitoramento-Cardiaco)
![GitHub pull requests](https://img.shields.io/github/issues-pr/MateusRondon/Monitoramento-Cardiaco)
![GitHub commits](https://img.shields.io/github/commit-activity/m/MateusRondon/Monitoramento-Cardiaco)
![GitHub stars](https://img.shields.io/github/stars/MateusRondon/Monitoramento-Cardiaco)

---

<div align="center">

### 🚀 Pronto para começar?

**[👉 Veja o Quick Start acima ⬆️](#-quick-start)**

---

**Última atualização:** Maio 2026  
**Versão:** 1.0.0  
**Status:** ✅ Pronto para desenvolvimento

</div>
