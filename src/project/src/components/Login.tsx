import React, { useState, FormEvent } from 'react';
import { useNavigate } from 'react-router-dom';
import { Heart } from 'lucide-react';
import { useAuth } from '../contexts/AuthContext';
import Card from './ui/Card';
import Input from './ui/Input';
import Button from './ui/Button';

const Login: React.FC = () => {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const { login, error } = useAuth();
  const navigate = useNavigate();

  const handleSubmit = async (e: FormEvent) => {
    e.preventDefault();
    setIsLoading(true);
    
    try {
      const success = await login(username, password);
      if (success) {
        navigate('/dashboard');
      }
    } finally {
      setIsLoading(false);
    }
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-blue-100 flex flex-col justify-center items-center p-4">
      <div className="w-full max-w-md">
        <div className="text-center mb-10">
          <div className="flex justify-center mb-4">
            <Heart className="h-12 w-12 text-blue-600 animate-pulse" />
          </div>
          <h1 className="text-3xl font-bold text-gray-900">Health Monitor</h1>
          <p className="mt-2 text-gray-600">Sign in to access patient vital monitoring</p>
        </div>
        
        <Card className="mb-6">
          <form onSubmit={handleSubmit}>
            <div className="space-y-4">
              <Input
                label="Username"
                id="username"
                name="username"
                type="text"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                required
                autoComplete="username"
              />
              
              <Input
                label="Password"
                id="password"
                name="password"
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                required
                autoComplete="current-password"
              />
              
              {error && (
                <div className="bg-red-50 text-red-600 p-3 rounded text-sm">
                  {error}
                </div>
              )}
              
              <div className="pt-2">
                <Button
                  type="submit"
                  fullWidth
                  disabled={isLoading}
                >
                  {isLoading ? 'Signing in...' : 'Sign in'}
                </Button>
              </div>
            </div>
          </form>
        </Card>
        
        <div className="text-center text-sm text-gray-500">
          <p>Demo credentials:</p>
          <p className="mt-1">Username: <code>admin</code> | Password: <code>admin123</code></p>
          <p className="mt-1">Username: <code>doctor</code> | Password: <code>doctor123</code></p>
          <p className="mt-1">Username: <code>nurse</code> | Password: <code>nurse123</code></p>
        </div>
      </div>
    </div>
  );
};

export default Login;