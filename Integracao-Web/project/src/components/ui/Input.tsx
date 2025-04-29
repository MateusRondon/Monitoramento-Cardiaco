import React, { forwardRef } from 'react';

type InputProps = {
  type?: string;
  label?: string;
  placeholder?: string;
  error?: string;
  id?: string;
  name?: string;
  value?: string;
  onChange?: (e: React.ChangeEvent<HTMLInputElement>) => void;
  required?: boolean;
  disabled?: boolean;
  className?: string;
  autoComplete?: string;
};

const Input = forwardRef<HTMLInputElement, InputProps>(
  ({
    type = 'text',
    label,
    placeholder,
    error,
    id,
    name,
    value,
    onChange,
    required = false,
    disabled = false,
    className = '',
    autoComplete,
    ...rest
  }, ref) => {
    return (
      <div className={`w-full ${className}`}>
        {label && (
          <label 
            htmlFor={id || name} 
            className="block text-sm font-medium text-gray-700 mb-1"
          >
            {label} {required && <span className="text-red-500">*</span>}
          </label>
        )}
        
        <input
          ref={ref}
          type={type}
          id={id || name}
          name={name}
          value={value}
          onChange={onChange}
          placeholder={placeholder}
          disabled={disabled}
          required={required}
          className={`w-full px-4 py-2 border rounded-md text-gray-700 focus:outline-none focus:ring-2 focus:border-transparent
            ${error ? 'border-red-500 focus:ring-red-200' : 'border-gray-300 focus:ring-blue-200'}
            ${disabled ? 'bg-gray-100 cursor-not-allowed' : 'bg-white'}
          `}
          autoComplete={autoComplete}
          {...rest}
        />
        
        {error && (
          <p className="mt-1 text-sm text-red-600">{error}</p>
        )}
      </div>
    );
  }
);

export default Input;