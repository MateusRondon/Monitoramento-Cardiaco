import React from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import { AuthProvider, useAuth } from './contexts/AuthContext';
import { SensorProvider } from './contexts/SensorContext';
import Login from './components/Login';
import Layout from './components/Layout';
import Dashboard from './components/Dashboard';
import Patients from './components/Patients';
import Settings from './components/Settings';

// Protected route component
const ProtectedRoute: React.FC<{ element: React.ReactNode }> = ({ element }) => {
  const { isAuthenticated } = useAuth();
  return isAuthenticated ? <>{element}</> : <Navigate to="/" replace />;
};

// Admin only route component
const AdminRoute: React.FC<{ element: React.ReactNode }> = ({ element }) => {
  const { user, isAuthenticated } = useAuth();
  
  if (!isAuthenticated) {
    return <Navigate to="/" replace />;
  }
  
  return user?.role === 'admin' ? <>{element}</> : <Navigate to="/dashboard" replace />;
};

function App() {
  return (
    <BrowserRouter>
      <AuthProvider>
        <SensorProvider>
          <Routes>
            <Route path="/" element={<Login />} />
            
            <Route path="/" element={
              <ProtectedRoute element={<Layout />} />
            }>
              <Route path="dashboard" element={<Dashboard />} />
              <Route path="patients" element={<Patients />} />
              <Route path="settings" element={
                <AdminRoute element={<Settings />} />
              } />
            </Route>
            
            <Route path="*" element={<Navigate to="/" replace />} />
          </Routes>
        </SensorProvider>
      </AuthProvider>
    </BrowserRouter>
  );
}

export default App;