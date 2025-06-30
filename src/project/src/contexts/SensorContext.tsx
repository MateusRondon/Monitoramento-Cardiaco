import React, { createContext, useContext, useState, useEffect } from 'react';

type SensorData = {
  heartRate: number;
  spO2: number;
  timestamp: string;
};

type PatientData = {
  id: number;
  name: string;
  age: number;
  room: string;
  sensorHistory: SensorData[];
  currentData: SensorData;
};

type SensorContextType = {
  patients: PatientData[];
  selectedPatientId: number | null;
  selectPatient: (id: number) => void;
  isConnected: boolean;
  toggleConnection: () => void;
};

const SensorContext = createContext<SensorContextType | undefined>(undefined);

// Initial demo patients with empty data
const generateDemoPatients = (): PatientData[] => {
  const initialData = {
    heartRate: 0,
    spO2: 0,
    timestamp: new Date().toISOString()
  };
  
  return [
    {
      id: 1,
      name: 'John Smith',
      age: 68,
      room: 'A-101',
      sensorHistory: [],
      currentData: initialData
    },
    {
      id: 2,
      name: 'Maria Garcia',
      age: 54,
      room: 'B-205',
      sensorHistory: [],
      currentData: initialData
    },
    {
      id: 3,
      name: 'Robert Chen',
      age: 75,
      room: 'A-112',
      sensorHistory: [],
      currentData: initialData
    }
  ];
};

export const SensorProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [patients, setPatients] = useState<PatientData[]>(generateDemoPatients());
  const [selectedPatientId, setSelectedPatientId] = useState<number | null>(1);
  const [isConnected, setIsConnected] = useState<boolean>(false);
  
  const selectPatient = (id: number) => {
    setSelectedPatientId(id);
  };
  
  const toggleConnection = () => {
    setIsConnected(prev => !prev);
  };
  
  // Fetch real ESP32 data
  useEffect(() => {
    if (!isConnected) return;
    
    const fetchData = async () => {
      try {
        const response = await fetch('http://your-esp32-ip/data');
        const data = await response.json();
        
        setPatients(currentPatients => 
          currentPatients.map(patient => {
            if (patient.id === selectedPatientId) {
              const newReading = {
                heartRate: data.heart_rate,
                spO2: data.spo2,
                timestamp: new Date().toISOString()
              };
              
              return {
                ...patient,
                currentData: newReading,
                sensorHistory: [...patient.sensorHistory, patient.currentData].slice(-20)
              };
            }
            return patient;
          })
        );
      } catch (error) {
        console.error('Error fetching sensor data:', error);
        setIsConnected(false);
      }
    };
    
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
  }, [isConnected, selectedPatientId]);
  
  return (
    <SensorContext.Provider value={{
      patients,
      selectedPatientId,
      selectPatient,
      isConnected,
      toggleConnection
    }}>
      {children}
    </SensorContext.Provider>
  );
};

export const useSensor = (): SensorContextType => {
  const context = useContext(SensorContext);
  if (!context) {
    throw new Error('useSensor must be used within a SensorProvider');
  }
  return context;
};