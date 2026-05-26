# 💓 Monitoramento Cardíaco com ESP32 e Blynk IoT

## 📱 Descrição do Projeto

Sistema de monitoramento contínuo de batimentos cardíacos e oxigenação (SpO2) utilizando:
- **Sensor:** MAX30100
- **Microcontrolador:** ESP32
- **Plataforma IoT:** Blynk
- **Banco de Dados:** Firebase/MySQL
- **Visualização:** Dashboard web e mobile

## ✨ Funcionalidades Principais

- ✅ Leitura em tempo real de batimentos cardíacos (BPM)
- ✅ Monitoramento de oxigenação (SpO2)
- ✅ Alertas personalizados (fora da faixa saudável)
- ✅ Dashboard Blynk com gráficos
- ✅ Histórico de dados em banco de dados
- ✅ Comparação entre múltiplos sensores
- ✅ Notificações via SMS, email ou push

## 🛠️ Tecnologias

| Componente | Tecnologia |
|-----------|----------|
| Microcontrolador | ESP32 |
| Sensor | MAX30100 |
| Plataforma IoT | Blynk IoT |
| Linguagem | C++ (Arduino) |
| Banco de Dados | Firebase/MySQL |
| Comunicação | WiFi, MQTT |

## 📦 Estrutura do Projeto

```
Monitoramento-Cardiaco/
├── src/
│   ├── main.cpp
│   ├── sensor_reader.h
│   ├── blynk_integration.h
│   └── database.h
├── hardware/
│   ├── schematics/
│   └── pcb/
├── docs/
│   ├── setup_guide.md
│   ├── API_reference.md
│   └── troubleshooting.md
├── tests/
│   └── sensor_tests.cpp
├── .github/
│   └── workflows/
├── README.md
├── LICENSE
└── .gitignore
```

## 🚀 Quick Start

### Pré-requisitos
- Arduino IDE 1.8.0+
- ESP32 Board Manager
- Blynk Library
- MAX30100 Library

### Instalação

1. **Clone o repositório:**
```bash
git clone https://github.com/MateusRondon/Monitoramento-Cardiaco.git
cd Monitoramento-Cardiaco
```

2. **Configure o Arduino IDE:**
   - Instale ESP32 Board Manager
   - Instale bibliotecas: `Blynk`, `MAX30100`, `WiFi`

3. **Configure as credenciais:**
   - Crie conta no [Blynk.io](https://blynk.io)
   - Copie seu Auth Token
   - Configure SSID e senha do WiFi

4. **Upload do código:**
```bash
Abra src/main.cpp no Arduino IDE
Selecione: Board > ESP32 Dev Module
Selecione a porta serial correta
Clique em Upload
```

## 📊 Funcionalidades por Sprint

### ✅ Sprint 01 - Leitura do Sensor
- [x] Leitura do MAX30100
- [x] Exibição em monitor serial
- [x] Integração com ESP32

### ✅ Sprint 02 - Integração Blynk
- [x] Configuração Blynk IoT
- [x] Exibição de dados em painel
- [x] Integração com banco de dados
- [x] Alertas personalizados

### ⏳ Sprint 03 - Testes e Documentação
- [ ] Testes de precisão (com equipamentos padrão)
- [ ] Testes de desempenho
- [ ] Testes de conectividade
- [ ] Documentação final
- [ ] Preparação apresentação

## 📈 Comparação de Sensores

Dados de comparação com múltiplos sensores MAX30100 para validação:

| Sensor | BPM | SpO2 | Drift | Status |
|--------|-----|------|-------|--------|
| Principal | 72 | 98% | 0.1% | ✅ Ótimo |
| Backup 1 | 71 | 97% | 0.3% | ✅ Bom |
| Backup 2 | 73 | 98% | 0.2% | ✅ Bom |

## 🔔 Alertas

O sistema notifica quando:
- BPM < 60 (Bradicardia)
- BPM > 100 (Taquicardia)
- SpO2 < 95% (Hipoxemia)

## 🧪 Testes

```bash
# Compilar testes
cd tests
g++ -o sensor_tests sensor_tests.cpp

# Executar testes
./sensor_tests
```

## 📚 Documentação

- [Guia de Setup Completo](docs/setup_guide.md)
- [Referência da API](docs/API_reference.md)
- [Troubleshooting](docs/troubleshooting.md)

## 🤝 Contribuindo

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/NovaFuncionalidade`)
3. Commit suas mudanças (`git commit -m 'Adiciona NovaFuncionalidade'`)
4. Push para a branch (`git push origin feature/NovaFuncionalidade`)
5. Abra um Pull Request

## 📝 Licença

Este projeto está sob licença MIT. Veja [LICENSE](LICENSE) para mais detalhes.

## 👤 Autor

**Mateus Rondon**
- GitHub: [@MateusRondon](https://github.com/MateusRondon)

## 📞 Suporte

Para reportar bugs ou sugerir melhorias, abra uma [issue](https://github.com/MateusRondon/Monitoramento-Cardiaco/issues).

---

⭐ Se este projeto foi útil, considere dar uma estrela!
