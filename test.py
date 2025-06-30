import serial
import matplotlib.pyplot as plt
import time

# Configurações da porta serial
PORT = '/dev/ttyUSB0'  # Atualize para o nome correto da sua porta serial
BAUD_RATE = 115200

# Conectar à porta serial
ser = serial.Serial(PORT, BAUD_RATE)

# Listas para armazenar os dados
bpm_values = []
spo2_values = []
timestamps = []

# Contador de tempo
start_time = time.time()

# Lê os dados da porta serial e armazena
try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        
        if "BPM" in line and "SpO2" in line:
            # Extrai os valores de BPM e SpO2
            parts = line.split(", ")
            bpm = int(parts[0].split(": ")[1])
            spo2 = int(parts[1].split(": ")[1])
            
            # Armazena os dados
            bpm_values.append(bpm)
            spo2_values.append(spo2)
            timestamps.append(time.time() - start_time)
            
            # Exibe os dados no console (opcional)
            print(f"BPM: {bpm}, SpO2: {spo2}")
            
            # Limite de dados para visualização (caso seja necessário)
            if len(bpm_values) > 100:
                break
        
        time.sleep(0.5)

except KeyboardInterrupt:
    # Usuário parou o script
    print("Leitura interrompida pelo usuário.")
    
finally:
    # Fechar a conexão serial
    ser.close()

# Gerar gráfico
plt.figure(figsize=(10, 6))
plt.plot(timestamps, bpm_values, label="BPM", color="r", linestyle='-', marker='o')
plt.plot(timestamps, spo2_values, label="SpO2", color="b", linestyle='--', marker='x')

# Adicionar título e rótulos aos eixos
plt.title('BPM e SpO2 ao longo do tempo')
plt.xlabel('Tempo (segundos)')
plt.ylabel('Valor')
plt.legend()

# Exibir o gráfico
plt.grid(True)
plt.show()
