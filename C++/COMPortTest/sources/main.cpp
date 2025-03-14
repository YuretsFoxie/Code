#include <windows.h>
#include <string>
#include <thread>
#include <bitset>
#include <atomic>
#include <iostream>

class COMPort
{
public:
    static COMPort& shared()
	{
		static COMPort instance;
		return instance;
	}
	
	void toggle();
	void send(const std::string& data);
	
private:	
	COMPort();
   ~COMPort();
	
	void start();
	void stop();
	bool open();
	void close();
	void setup();
	void receice();
	
	HANDLE hComm = INVALID_HANDLE_VALUE;
	std::string portName = "COM3";
	std::thread workerThread;
	std::atomic<bool> isReceiving;
};



// Public Functions

void COMPort::toggle()
{	
	isReceiving ? stop() : start();	
}

void COMPort::send(const std::string& data)
{	
	DWORD bytesWritten;	
	::WriteFile(hComm, data.c_str(), data.size(), &bytesWritten, NULL);
}

// Private Functions

void COMPort::start()
{
	std::cout << "start" << std::endl;
	isReceiving = true;
	workerThread = std::thread(&COMPort::receice, this);
}

void COMPort::stop()
{
	std::cout << "stop" << std::endl;
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
		std::cout << "error: port is not connected\n";
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

void COMPort::setup()
{
	DCB dcb = {0};
	dcb.DCBlength = sizeof(dcb);
	GetCommState(hComm, &dcb);
	dcb.BaudRate = CBR_4800;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	SetCommState(hComm, &dcb);
}

void COMPort::receice()
{
	char buffer[1];	
	DWORD bytesRead;
	
	while (isReceiving)
	{
		if (::ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL))
		{
			for (int i = 0; i < bytesRead; i++)
			{
				std::cout << "received: " << buffer[i] << " " << std::bitset<8>(buffer[i]) << "\n";
			}
		}
	}
}



int main()
{
	COMPort::shared().toggle();
	COMPort::shared().send("1");
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	COMPort::shared().toggle();
	
	std::cin.get();
	return 0;
}
