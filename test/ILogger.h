#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

class ILogger
{
public:
	virtual ~ILogger() = 0;
	virtual void LogMessage(std::string const& str) = 0;
};

inline ILogger::~ILogger(){}

#endif