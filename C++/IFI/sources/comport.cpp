#include <thread>
#include <chrono>
#include "comport.h"

// Public Functins

COMPort::COMPort(Settings& settings, DataBuffer& buffer1, DataBuffer& buffer2, Text& text, Sounds& sounds):
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
	run();
}

COMPort::~COMPort()
{
	close();
}

void COMPort::toggleTransmission()
{
	if (isOpened())
	{
		toggle();
		sounds.playButton();
	}
	else
	{
		sounds.playDisabled();
	}
}

// Private Functions

void COMPort::open()
{
	std::string name = settings.getPortName();
	handle = ::CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (isOpened())
	{
		text.print("COM port is opened.");
		sounds.playSuccess();
	}
	else
	{
		text.print("Error opening COM port.");
		sounds.playError();
	}
}

void COMPort::close()
{
	if (isOpened())
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
	std::thread portThread(&COMPort::read, this);
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
			::ReadFile(handle, array, 1, &bytesRead, NULL);
			if (bytesRead > 0)
			{
				isPushedToBuffer1 ? buffer2.push(array[0]) : buffer1.push(array[0]);
				isPushedToBuffer1 = !isPushedToBuffer1;
			}
		}
	}
}

void COMPort::toggle()
{
	char cmd = isReceiving ? '0' : '1';
	isReceiving = !isReceiving;
	DWORD bytesWritten;
	::WriteFile(handle, &cmd, 1, &bytesWritten, NULL);
}

bool COMPort::isOpened()
{
	return handle != INVALID_HANDLE_VALUE;
}
