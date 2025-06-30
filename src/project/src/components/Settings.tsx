import React, { useState } from 'react';
import { Settings as SettingsIcon, Save, RefreshCw } from 'lucide-react';
import Card from './ui/Card';
import Button from './ui/Button';
import Input from './ui/Input';

type DeviceSettings = {
  deviceName: string;
  updateInterval: string;
  wifiSSID: string;
  alertThresholds: {
    highHeartRate: string;
    lowHeartRate: string;
    lowSpO2: string;
  };
};

const Settings: React.FC = () => {
  const [settings, setSettings] = useState<DeviceSettings>({
    deviceName: 'ESP32-MAX30102',
    updateInterval: '5',
    wifiSSID: 'Hospital-Network',
    alertThresholds: {
      highHeartRate: '100',
      lowHeartRate: '60',
      lowSpO2: '95'
    }
  });
  
  const [isSaving, setIsSaving] = useState(false);
  const [isSaved, setIsSaved] = useState(false);
  
  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const { name, value } = e.target;
    
    if (name.includes('.')) {
      const [parent, child] = name.split('.');
      setSettings(prev => ({
        ...prev,
        [parent]: {
          ...prev[parent as keyof DeviceSettings],
          [child]: value
        }
      }));
    } else {
      setSettings(prev => ({
        ...prev,
        [name]: value
      }));
    }
    
    if (isSaved) setIsSaved(false);
  };
  
  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setIsSaving(true);
    
    // Simulate API call to save settings
    setTimeout(() => {
      setIsSaving(false);
      setIsSaved(true);
    }, 800);
  };
  
  const handleTestConnection = () => {
    setIsSaving(true);
    
    // Simulate API call to test connection
    setTimeout(() => {
      setIsSaving(false);
      alert('Successfully connected to device');
    }, 1000);
  };
  
  return (
    <div className="space-y-6">
      <div className="flex items-center justify-between">
        <h1 className="text-2xl font-bold text-gray-900">System Settings</h1>
        <div>
          <SettingsIcon className="h-6 w-6 text-blue-600" />
        </div>
      </div>
      
      <form onSubmit={handleSubmit}>
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          <Card title="Device Configuration">
            <div className="space-y-4">
              <Input
                label="Device Name"
                name="deviceName"
                value={settings.deviceName}
                onChange={handleChange}
              />
              
              <Input
                label="Data Update Interval (seconds)"
                name="updateInterval"
                type="number"
                min="1"
                max="60"
                value={settings.updateInterval}
                onChange={handleChange}
              />
              
              <Input
                label="WiFi Network SSID"
                name="wifiSSID"
                value={settings.wifiSSID}
                onChange={handleChange}
              />
              
              <div className="pt-2">
                <Button
                  type="button"
                  variant="outline"
                  onClick={handleTestConnection}
                  disabled={isSaving}
                  className="flex items-center"
                >
                  <RefreshCw className="h-4 w-4 mr-2" />
                  Test Connection
                </Button>
              </div>
            </div>
          </Card>
          
          <Card title="Alert Thresholds">
            <div className="space-y-4">
              <Input
                label="High Heart Rate Threshold (BPM)"
                name="alertThresholds.highHeartRate"
                type="number"
                min="80"
                max="200"
                value={settings.alertThresholds.highHeartRate}
                onChange={handleChange}
              />
              
              <Input
                label="Low Heart Rate Threshold (BPM)"
                name="alertThresholds.lowHeartRate"
                type="number"
                min="30"
                max="70"
                value={settings.alertThresholds.lowHeartRate}
                onChange={handleChange}
              />
              
              <Input
                label="Low SpO2 Threshold (%)"
                name="alertThresholds.lowSpO2"
                type="number"
                min="80"
                max="96"
                value={settings.alertThresholds.lowSpO2}
                onChange={handleChange}
              />
            </div>
          </Card>
        </div>
        
        <div className="mt-6 flex justify-end">
          {isSaved && (
            <div className="mr-4 text-green-600 flex items-center">
              Settings saved successfully!
            </div>
          )}
          
          <Button
            type="submit"
            disabled={isSaving}
            className="flex items-center"
          >
            <Save className="h-4 w-4 mr-2" />
            {isSaving ? 'Saving...' : 'Save Settings'}
          </Button>
        </div>
      </form>
    </div>
  );
};

export default Settings;