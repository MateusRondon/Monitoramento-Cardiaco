import React from 'react';
import { Users, Heart, Activity, ChevronRight } from 'lucide-react';
import { useSensor } from '../contexts/SensorContext';
import Card from './ui/Card';
import Button from './ui/Button';
import { useNavigate } from 'react-router-dom';

const Patients: React.FC = () => {
  const { patients, selectPatient } = useSensor();
  const navigate = useNavigate();
  
  const handleViewPatient = (id: number) => {
    selectPatient(id);
    navigate('/dashboard');
  };
  
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
  
  return (
    <div className="space-y-6">
      <div className="flex items-center justify-between">
        <h1 className="text-2xl font-bold text-gray-900">Patients</h1>
        <div>
          <Users className="h-6 w-6 text-blue-600" />
        </div>
      </div>
      
      <div className="grid grid-cols-1 gap-6">
        {patients.map(patient => (
          <Card key={patient.id} className="hover:shadow-lg transition-shadow">
            <div className="flex flex-col sm:flex-row justify-between">
              <div className="mb-4 sm:mb-0">
                <h3 className="text-lg font-semibold">{patient.name}</h3>
                <div className="mt-2 grid grid-cols-2 gap-x-6 gap-y-2">
                  <div>
                    <p className="text-sm text-gray-500">Age</p>
                    <p>{patient.age}</p>
                  </div>
                  <div>
                    <p className="text-sm text-gray-500">Room</p>
                    <p>{patient.room}</p>
                  </div>
                  <div>
                    <p className="text-sm text-gray-500">Last Updated</p>
                    <p className="text-sm">
                      {new Date(patient.currentData.timestamp).toLocaleTimeString()}
                    </p>
                  </div>
                </div>
              </div>
              
              <div className="flex flex-row sm:flex-col justify-between">
                <div className="flex flex-col items-center">
                  <div className="flex items-center">
                    <Heart className="h-5 w-5 text-red-500 mr-1" />
                    <span className={`text-lg font-semibold ${getHeartRateStatus(patient.currentData.heartRate)}`}>
                      {patient.currentData.heartRate}
                    </span>
                    <span className="text-sm ml-1">BPM</span>
                  </div>
                  
                  <div className="flex items-center mt-2">
                    <Activity className="h-5 w-5 text-blue-500 mr-1" />
                    <span className={`text-lg font-semibold ${getSpO2Status(patient.currentData.spO2)}`}>
                      {patient.currentData.spO2}
                    </span>
                    <span className="text-sm ml-1">%</span>
                  </div>
                </div>
                
                <div className="mt-4 sm:mt-auto">
                  <Button 
                    variant="primary"
                    size="sm"
                    onClick={() => handleViewPatient(patient.id)}
                    className="flex items-center"
                  >
                    View <ChevronRight className="h-4 w-4 ml-1" />
                  </Button>
                </div>
              </div>
            </div>
          </Card>
        ))}
      </div>
    </div>
  );
};

export default Patients;