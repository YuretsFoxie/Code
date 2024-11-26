#ifndef COMPORT_H_INCLUDED
#define COMPORT_H_INCLUDED

#include <windows.h>
#include <string>
#include <iostream>

class COMPort
{
public:
	COMPort(): handle(INVALID_HANDLE_VALUE) {}
   ~COMPort();
	
	void setup(const std::string& portName, int baudRate);
	void toggleDataTransmission(bool enable);
	HANDLE getHandle() const;
	
private:
	void openPort(const std::string& portName);
	void configurePort(int baudRate);
	void setPortTimeouts();
	void closePort();

    HANDLE handle;
};

#endif // COMPORT_H_INCLUDED
