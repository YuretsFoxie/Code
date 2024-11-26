#include "comport.h"

// Public Functins

COMPort::~COMPort()
{
	closePort();
}

void COMPort::setup(const std::string& portName, int baudRate)
{
	openPort(portName);
	configurePort(baudRate);
	setPortTimeouts();
}

void COMPort::toggleDataTransmission(bool enable)
{
	char cmd = enable ? '1' : '0';
	DWORD bytesWritten;
	::WriteFile(handle, &cmd, 1, &bytesWritten, NULL);
}
	
HANDLE COMPort::getHandle() const
{
	return handle;
}
	
// Private Functions

void COMPort::openPort(const std::string& portName)
{
	handle = ::CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
		std::cerr << "Error opening COM port." << std::endl;
}

void COMPort::configurePort(int baudRate)
{
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	::GetCommState(handle, &dcbSerialParams);
	dcbSerialParams.BaudRate = baudRate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	::SetCommState(handle, &dcbSerialParams);
}

void COMPort::setPortTimeouts()
{
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	::SetCommTimeouts(handle, &timeouts);
}

void COMPort::closePort()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
}
