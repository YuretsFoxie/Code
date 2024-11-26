#include "comport.h"
#include "application.h"

// Public Functions

void COMPort::toggle()
{	
	isReceiving ? stop() : start();	
}

// Private Functions

void COMPort::start()
{
	workerThread = std::thread(&COMPort::receice, this);
	send("1");
	isReceiving = true;
}

void COMPort::stop()
{
	isReceiving = false;
	
	if (workerThread.joinable())
		workerThread.join();
}

COMPort::COMPort()
{
	open();
}

COMPort::~COMPort()
{
	if (isReceiving)
		stop();
	
	close();
}

bool COMPort::open()
{
	hComm = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	if (hComm == INVALID_HANDLE_VALUE)
	{
		Application::shared().showText("error: port is not connected.");
		return false;
	}
	
	setup();
	return true;
}

void COMPort::close()
{
	if (hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
	}
}

void COMPort::send(const std::string& data)
{
	DWORD bytesWritten;
	WriteFile(hComm, data.c_str(), data.size(), &bytesWritten, NULL);
}

void COMPort::setup()
{
	DCB dcb = {0};
	dcb.DCBlength = sizeof(dcb);
	GetCommState(hComm, &dcb);
	dcb.BaudRate = CBR_57600;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	SetCommState(hComm, &dcb);
}

void COMPort::receice()
{
	// char buffer[64]; // we wait, until the buffer is filled, this causes a delay
	char buffer[1];
	
	DWORD bytesRead;
	
	while (isReceiving)
		if (ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL))
			for (int i = 0; i < bytesRead; i++)
			{
				int value = convertTwosComplementToInt(std::bitset<8>(buffer[i]));
				Application::shared().onReceived(value);
			}
			
	if (!isReceiving)
		for (int i = 0; i < 50; i++)
		{
			::Sleep(1);
			send("0");
		}
}

int COMPort::convertTwosComplementToInt(const std::bitset<8> byte)
{
	return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}
