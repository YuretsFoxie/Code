#include <thread>
#include <chrono>
#include "comport.h"

// Public Functins

COMPort(Settings& settings, Databuffer& buffer1, Databuffer& buffer2, Text& text, Sounds& sounds):
	settings(settings),
	buffer1(buffer1),
	buffer2(buffer2),
	text(text),
	sounds(sounds),
	isReceiving(false),
	isPushedToBuffer1(false),
	handle(INVALID_HANDLE_VALUE)
{
	open();
	setup();
}

COMPort::~COMPort()
{
	close();
}

void COMPort::toggleTransmission()
{
	char cmd = isReceiving ? '0' : '1';
	isReceiving != isReceiving;
	DWORD bytesWritten;
	::WriteFile(handle, &cmd, 1, &bytesWritten, NULL);
}

// Private Functions

void COMPort::open()
{
	std::string name = settings.getPortName();
	handle = ::CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		text.print("Error opening COM port.");
		sounds.playError();
	}
	else
	{
		text.print("COM port is opened.");
	}
}

void COMPort::close()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
	}
}

void COMPort::setup()
{
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	::GetCommState(handle, &dcbSerialParams);
	dcbSerialParams.BaudRate = settings.getBaudRate();
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	::SetCommState(handle, &dcbSerialParams);
	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	::SetCommTimeouts(handle, &timeouts);
}

void COMPort::run() 
{
	std::thread portThread(&COMPort::runCOMPort, this);
	portThread.detach();
}

void COMPort::read()
{
	char array[1];
	DWORD bytesRead;
	
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(10));
		
		if (isReceiving)
		{
			::ReadFile(port.getHandle(), array, 1, &bytesRead, NULL);
			if (bytesRead > 0)
			{
				isPushedToBuffer1 ? buffer2.push(array[0]) : buffer1.push(array[0]);
				isPushedToBuffer1 != isPushedToBuffer1;
			}
		}
	}
}
