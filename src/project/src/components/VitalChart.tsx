import React from 'react';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
} from 'chart.js';
import { Line } from 'react-chartjs-2';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
);

type SensorData = {
  heartRate: number;
  spO2: number;
  timestamp: string;
};

type VitalChartProps = {
  data: SensorData[];
  vitalType: 'heartRate' | 'spO2';
  label: string;
  color: string;
};

const VitalChart: React.FC<VitalChartProps> = ({ 
  data, 
  vitalType,
  label,
  color
}) => {
  const chartData = {
    labels: data.map(item => new Date(item.timestamp).toLocaleTimeString()),
    datasets: [
      {
        label,
        data: data.map(item => item[vitalType]),
        borderColor: color,
        backgroundColor: color.replace('0.7', '0.1'),
        fill: true,
        tension: 0.3,
        pointRadius: 3,
        pointHoverRadius: 5,
      },
    ],
  };

  const options = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: {
        display: false,
      },
      tooltip: {
        mode: 'index' as const,
        intersect: false,
      },
    },
    scales: {
      y: {
        min: vitalType === 'heartRate' ? 50 : 85,
        max: vitalType === 'heartRate' ? 110 : 100,
        grid: {
          color: 'rgba(0, 0, 0, 0.05)',
        },
      },
      x: {
        grid: {
          display: false,
        },
      },
    },
    animation: {
      duration: 1000,
    },
  };

  return (
    <div className="h-64">
      <Line data={chartData} options={options} />
    </div>
  );
};

export default VitalChart;