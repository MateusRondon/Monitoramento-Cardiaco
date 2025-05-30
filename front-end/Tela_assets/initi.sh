#!/bin/bash

# CONFIGURAÇÕES
REPO_URL="https://github.com/MateusRondon/Monitoramento-Cardiaco.git"

FQBN="esp32:esp32:esp32dev"       # Altere conforme sua placa
PORT="/dev/ttyUSB0"               # Altere conforme sua porta
SKETCH_NAME="meu_codigo"          # Nome do .ino (sem .ino)

# 1. Instalar arduino-cli se necessário
if ! command -v arduino-cli &> /dev/null; then
    echo "🔧 Instalando arduino-cli..."
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
    export PATH=$PATH:~/bin
fi

# 2. Inicializa config da CLI
arduino-cli config init > /dev/null 2>&1
arduino-cli core update-index

# 3. Instala o core da placa
arduino-cli core install $FQBN

# 4. Clona o repositório
echo "📦 Clonando repositório..."
git clone "$REPO_URL"
cd "$(basename "$REPO_URL" .git)"

# 5. Compila o código
echo "⚙️  Compilando o sketch..."
arduino-cli compile --fqbn $FQBN "$SKETCH_NAME"

# 6. Envia para a placa
echo "🚀 Enviando para a placa na porta $PORT..."
arduino-cli upload -p $PORT --fqbn $FQBN "$SKETCH_NAME"

echo "✅ Upload concluído com sucesso!"
