import React from 'react';

type CardProps = {
  children: React.ReactNode;
  title?: string;
  subtitle?: string;
  className?: string;
  headerRight?: React.ReactNode;
};

const Card: React.FC<CardProps> = ({
  children,
  title,
  subtitle,
  className = '',
  headerRight
}) => {
  return (
    <div className={`bg-white rounded-lg shadow-md overflow-hidden ${className}`}>
      {(title || subtitle) && (
        <div className="border-b border-gray-200 px-6 py-4 flex justify-between items-center">
          <div>
            {title && <h3 className="text-lg font-medium text-gray-900">{title}</h3>}
            {subtitle && <p className="mt-1 text-sm text-gray-500">{subtitle}</p>}
          </div>
          {headerRight && (
            <div className="flex items-center">
              {headerRight}
            </div>
          )}
        </div>
      )}
      <div className="p-6">{children}</div>
    </div>
  );
};

export default Card;