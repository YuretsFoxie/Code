#include "comportadapter.h"

// Public Functins

void COMPortAdapter::setup(const std::string& portName, int baudRate)
{
	comPort.setup(portName, baudRate);
}

void COMPortAdapter::toggleDataTransmission(bool enable)
{
	comPort.toggleDataTransmission(enable);
}

HANDLE COMPortAdapter::getHandle() const
{
	return comPort.getHandle();
}
