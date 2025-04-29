import React, { createContext, useContext, useState, useEffect } from 'react';

type User = {
  id: number;
  username: string;
  email: string;
  role: 'admin' | 'doctor' | 'nurse';
};

type AuthContextType = {
  user: User | null;
  isAuthenticated: boolean;
  login: (username: string, password: string) => Promise<boolean>;
  logout: () => void;
  error: string | null;
};

const AuthContext = createContext<AuthContextType | undefined>(undefined);

// Demo users for simulation
const DEMO_USERS = [
  { id: 1, username: 'admin', password: 'admin123', email: 'admin@hospital.com', role: 'admin' as const },
  { id: 2, username: 'doctor', password: 'doctor123', email: 'doctor@hospital.com', role: 'doctor' as const },
  { id: 3, username: 'nurse', password: 'nurse123', email: 'nurse@hospital.com', role: 'nurse' as const },
];

export const AuthProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [user, setUser] = useState<User | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    // Check for saved user in localStorage on initial load
    const savedUser = localStorage.getItem('user');
    if (savedUser) {
      setUser(JSON.parse(savedUser));
    }
  }, []);

  const login = async (username: string, password: string): Promise<boolean> => {
    // Simulate API request delay
    await new Promise(resolve => setTimeout(resolve, 800));
    
    setError(null);
    
    // Find user with matching credentials
    const matchedUser = DEMO_USERS.find(
      u => u.username === username && u.password === password
    );
    
    if (matchedUser) {
      // Create a copy without the password
      const { password: _, ...userWithoutPassword } = matchedUser;
      setUser(userWithoutPassword);
      
      // Save to localStorage
      localStorage.setItem('user', JSON.stringify(userWithoutPassword));
      return true;
    } else {
      setError('Invalid username or password');
      return false;
    }
  };
  
  const logout = () => {
    setUser(null);
    localStorage.removeItem('user');
  };
  
  return (
    <AuthContext.Provider value={{ 
      user, 
      isAuthenticated: !!user, 
      login, 
      logout,
      error
    }}>
      {children}
    </AuthContext.Provider>
  );
};

export const useAuth = (): AuthContextType => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
};