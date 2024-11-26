#ifndef COMPORTADAPTER_H_INCLUDED
#define COMPORTADAPTER_H_INCLUDED

#include "comport.h"

class COMPortAdapter
{
public:
	COMPortAdapter(): comPort() {}
	
	void setup(const std::string& portName, int baudRate);
	void toggleDataTransmission(bool enable);
	HANDLE getHandle() const;
	
private:
	COMPort comPort;
};

#endif // COMPORTADAPTER_H_INCLUDED
