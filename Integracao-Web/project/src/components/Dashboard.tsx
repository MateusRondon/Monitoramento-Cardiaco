import React from 'react';
import { Activity, Heart, HeartPulseIcon as PulseIcon, PercentIcon, AlarmClock, Wifi, WifiOff } from 'lucide-react';
import { useSensor } from '../contexts/SensorContext';
import Card from './ui/Card';
import Button from './ui/Button';
import VitalChart from './VitalChart';

const Dashboard: React.FC = () => {
  const { 
    patients, 
    selectedPatientId, 
    selectPatient,
    isConnected,
    toggleConnection
  } = useSensor();
  
  const selectedPatient = patients.find(p => p.id === selectedPatientId);
  
  // Function to get status color based on vital values
  const getHeartRateStatus = (rate: number) => {
    if (rate < 60) return 'text-blue-600';
    if (rate > 100) return 'text-red-600';
    return 'text-green-600';
  };
  
  const getSpO2Status = (level: number) => {
    if (level < 90) return 'text-red-600';
    if (level < 95) return 'text-yellow-600';
    return 'text-green-600';
  };
  
  if (!selectedPatient) {
    return <div>No patient selected</div>;
  }
  
  return (
    <div className="space-y-6">
      <div className="flex justify-between items-center">
        <h1 className="text-2xl font-bold text-gray-900">Patient Monitoring Dashboard</h1>
        <Button
          variant={isConnected ? 'success' : 'danger'}
          onClick={toggleConnection}
          className="flex items-center"
        >
          {isConnected ? (
            <>
              <Wifi className="mr-2 h-4 w-4" />
              ESP32 Connected
            </>
          ) : (
            <>
              <WifiOff className="mr-2 h-4 w-4" />
              ESP32 Disconnected
            </>
          )}
        </Button>
      </div>
      
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        {/* Patient selector */}
        <Card title="Select Patient" className="md:col-span-3">
          <div className="flex flex-wrap gap-3">
            {patients.map(patient => (
              <Button
                key={patient.id}
                variant={selectedPatientId === patient.id ? 'primary' : 'outline'}
                onClick={() => selectPatient(patient.id)}
              >
                {patient.name} ({patient.room})
              </Button>
            ))}
          </div>
        </Card>
        
        {/* Patient info */}
        <Card title="Patient Information">
          <div className="space-y-4">
            <div>
              <p className="text-sm text-gray-500">Name</p>
              <p className="font-medium">{selectedPatient.name}</p>
            </div>
            <div>
              <p className="text-sm text-gray-500">Age</p>
              <p className="font-medium">{selectedPatient.age}</p>
            </div>
            <div>
              <p className="text-sm text-gray-500">Room Number</p>
              <p className="font-medium">{selectedPatient.room}</p>
            </div>
            <div>
              <p className="text-sm text-gray-500">Last Updated</p>
              <p className="font-medium">
                {new Date(selectedPatient.currentData.timestamp).toLocaleTimeString()}
              </p>
            </div>
          </div>
        </Card>
        
        {/* Heart rate */}
        <Card 
          title="Heart Rate" 
          headerRight={<Heart className="h-5 w-5 text-red-500 animate-pulse" />}
        >
          <div className="text-center">
            <div className="flex items-center justify-center">
              <PulseIcon className="h-6 w-6 mr-2 text-red-500" />
              <span className={`text-4xl font-bold ${getHeartRateStatus(selectedPatient.currentData.heartRate)}`}>
                {selectedPatient.currentData.heartRate}
              </span>
              <span className="text-xl ml-1">BPM</span>
            </div>
            
            <div className="mt-6">
              <p className="text-sm text-gray-500">Normal Range</p>
              <p className="font-medium">60-100 BPM</p>
            </div>
            
            <div className="mt-4">
              <div className="h-2 bg-gray-200 rounded-full overflow-hidden">
                <div 
                  className={`h-full ${
                    selectedPatient.currentData.heartRate < 60 ? 'bg-blue-500' :
                    selectedPatient.currentData.heartRate > 100 ? 'bg-red-500' :
                    'bg-green-500'
                  }`}
                  style={{ 
                    width: `${Math.min(100, (selectedPatient.currentData.heartRate / 150) * 100)}%` 
                  }}
                ></div>
              </div>
            </div>
          </div>
        </Card>
        
        {/* SpO2 */}
        <Card 
          title="Blood Oxygen" 
          headerRight={<Activity className="h-5 w-5 text-blue-500" />}
        >
          <div className="text-center">
            <div className="flex items-center justify-center">
              <PercentIcon className="h-6 w-6 mr-2 text-blue-500" />
              <span className={`text-4xl font-bold ${getSpO2Status(selectedPatient.currentData.spO2)}`}>
                {selectedPatient.currentData.spO2}
              </span>
              <span className="text-xl ml-1">%</span>
            </div>
            
            <div className="mt-6">
              <p className="text-sm text-gray-500">Normal Range</p>
              <p className="font-medium">95-100%</p>
            </div>
            
            <div className="mt-4">
              <div className="h-2 bg-gray-200 rounded-full overflow-hidden">
                <div 
                  className={`h-full ${
                    selectedPatient.currentData.spO2 < 90 ? 'bg-red-500' :
                    selectedPatient.currentData.spO2 < 95 ? 'bg-yellow-500' :
                    'bg-green-500'
                  }`}
                  style={{ 
                    width: `${Math.max(0, Math.min(100, ((selectedPatient.currentData.spO2 - 85) / 15) * 100))}%` 
                  }}
                ></div>
              </div>
            </div>
          </div>
        </Card>
      </div>
      
      {/* Charts */}
      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
        <Card title="Heart Rate History" subtitle="Last 20 readings">
          <VitalChart 
            data={[...selectedPatient.sensorHistory, selectedPatient.currentData]}
            vitalType="heartRate"
            label="Heart Rate (BPM)"
            color="rgba(239, 68, 68, 0.7)"
          />
        </Card>
        
        <Card title="Blood Oxygen History" subtitle="Last 20 readings">
          <VitalChart 
            data={[...selectedPatient.sensorHistory, selectedPatient.currentData]}
            vitalType="spO2"
            label="SpO2 (%)"
            color="rgba(59, 130, 246, 0.7)"
          />
        </Card>
      </div>
      
      {/* Alerts section */}
      <Card 
        title="Recent Alerts" 
        headerRight={<AlarmClock className="h-5 w-5 text-yellow-500" />}
      >
        {selectedPatient.currentData.heartRate > 100 && (
          <div className="p-3 bg-red-50 border-l-4 border-red-500 rounded mb-2">
            <div className="flex">
              <div className="flex-shrink-0">
                <PulseIcon className="h-5 w-5 text-red-500" />
              </div>
              <div className="ml-3">
                <p className="text-sm text-red-700">
                  High heart rate detected: {selectedPatient.currentData.heartRate} BPM
                </p>
                <p className="text-xs text-red-500 mt-1">
                  {new Date(selectedPatient.currentData.timestamp).toLocaleTimeString()}
                </p>
              </div>
            </div>
          </div>
        )}
        
        {selectedPatient.currentData.heartRate < 60 && (
          <div className="p-3 bg-blue-50 border-l-4 border-blue-500 rounded mb-2">
            <div className="flex">
              <div className="flex-shrink-0">
                <PulseIcon className="h-5 w-5 text-blue-500" />
              </div>
              <div className="ml-3">
                <p className="text-sm text-blue-700">
                  Low heart rate detected: {selectedPatient.currentData.heartRate} BPM
                </p>
                <p className="text-xs text-blue-500 mt-1">
                  {new Date(selectedPatient.currentData.timestamp).toLocaleTimeString()}
                </p>
              </div>
            </div>
          </div>
        )}
        
        {selectedPatient.currentData.spO2 < 95 && (
          <div className="p-3 bg-yellow-50 border-l-4 border-yellow-500 rounded mb-2">
            <div className="flex">
              <div className="flex-shrink-0">
                <Activity className="h-5 w-5 text-yellow-500" />
              </div>
              <div className="ml-3">
                <p className="text-sm text-yellow-700">
                  Low blood oxygen level: {selectedPatient.currentData.spO2}%
                </p>
                <p className="text-xs text-yellow-500 mt-1">
                  {new Date(selectedPatient.currentData.timestamp).toLocaleTimeString()}
                </p>
              </div>
            </div>
          </div>
        )}
        
        {selectedPatient.currentData.heartRate >= 60 && 
         selectedPatient.currentData.heartRate <= 100 && 
         selectedPatient.currentData.spO2 >= 95 && (
          <p className="text-green-600">No alerts - all vitals within normal range</p>
        )}
      </Card>
    </div>
  );
};

export default Dashboard;