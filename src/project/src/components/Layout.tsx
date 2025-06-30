import React from 'react';
import { Outlet, NavLink, useNavigate } from 'react-router-dom';
import { Heart, Users, Settings, LogOut, Activity, User } from 'lucide-react';
import { useAuth } from '../contexts/AuthContext';

const Layout: React.FC = () => {
  const { user, logout } = useAuth();
  const navigate = useNavigate();

  const handleLogout = () => {
    logout();
    navigate('/');
  };

  return (
    <div className="flex h-screen bg-gray-50">
      {/* Sidebar */}
      <div className="w-64 bg-white shadow-md">
        <div className="p-4 border-b">
          <div className="flex items-center">
            <Heart className="h-6 w-6 text-blue-600 mr-2" />
            <h1 className="text-xl font-semibold text-gray-800">Health Monitor</h1>
          </div>
        </div>
        
        <div className="p-4">
          <div className="flex items-center mb-4">
            <div className="bg-blue-100 p-2 rounded-full">
              <User className="h-5 w-5 text-blue-700" />
            </div>
            <div className="ml-3">
              <p className="font-medium text-sm">{user?.username}</p>
              <p className="text-xs text-gray-500 capitalize">{user?.role}</p>
            </div>
          </div>
          
          <nav className="space-y-1">
            <NavLink
              to="/dashboard"
              className={({ isActive }) =>
                `flex items-center px-3 py-2 rounded-md text-sm font-medium transition-colors
                 ${isActive 
                   ? 'bg-blue-50 text-blue-700' 
                   : 'text-gray-700 hover:bg-gray-100'
                 }`
              }
            >
              <Activity className="h-5 w-5 mr-3" />
              Dashboard
            </NavLink>
            
            <NavLink
              to="/patients"
              className={({ isActive }) =>
                `flex items-center px-3 py-2 rounded-md text-sm font-medium transition-colors
                 ${isActive 
                   ? 'bg-blue-50 text-blue-700' 
                   : 'text-gray-700 hover:bg-gray-100'
                 }`
              }
            >
              <Users className="h-5 w-5 mr-3" />
              Patients
            </NavLink>
            
            {user?.role === 'admin' && (
              <NavLink
                to="/settings"
                className={({ isActive }) =>
                  `flex items-center px-3 py-2 rounded-md text-sm font-medium transition-colors
                   ${isActive 
                     ? 'bg-blue-50 text-blue-700' 
                     : 'text-gray-700 hover:bg-gray-100'
                   }`
                }
              >
                <Settings className="h-5 w-5 mr-3" />
                Settings
              </NavLink>
            )}
          </nav>
        </div>
        
        <div className="absolute bottom-0 w-64 p-4 border-t">
          <button
            onClick={handleLogout}
            className="flex items-center w-full px-3 py-2 text-sm font-medium text-gray-700 hover:bg-gray-100 rounded-md transition-colors"
          >
            <LogOut className="h-5 w-5 mr-3" />
            Sign out
          </button>
        </div>
      </div>
      
      {/* Main content area */}
      <div className="flex-1 overflow-auto">
        <div className="p-6">
          <Outlet />
        </div>
      </div>
    </div>
  );
};

export default Layout;