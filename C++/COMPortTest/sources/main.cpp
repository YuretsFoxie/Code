#include <windows.h>
#include <thread>
#include <iostream>
#include <bitset>

class COMPort {
public:
	COMPort(const std::string& portName) : portName(portName), hComm(INVALID_HANDLE_VALUE), stopThread(false) {}

	~COMPort()
	{
		stop();
	}

	bool open()
	{
		hComm = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		
		if (hComm == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Error opening COM port" << std::endl;
			return false;
		}
		
		setup();
		start();
		return true;
	}

	void close()
	{
		stop();
		
		if (hComm != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hComm);
			hComm = INVALID_HANDLE_VALUE;
		}
	}

	void send(const std::string& data)
	{
		DWORD bytesWritten;
		WriteFile(hComm, data.c_str(), data.size(), &bytesWritten, NULL);
	}

private:
	void setup()
	{
		DCB dcb = { 0 };
		dcb.DCBlength = sizeof(dcb);
		GetCommState(hComm, &dcb);
		dcb.BaudRate = CBR_57600;
		dcb.ByteSize = 8;
		dcb.StopBits = ONESTOPBIT;
		dcb.Parity = NOPARITY;
		SetCommState(hComm, &dcb);
	}

	void start()
	{
		stopThread = false;
		workerThread = std::thread(&COMPort::worker, this);
	}

	void stop()
	{
		stopThread = true;
		
		if (workerThread.joinable())
		{
			workerThread.join();
		}
	}
	
	void worker()
	{
		char buffer[64];
		DWORD bytesRead;
		
		while (!stopThread)
			if (ReadFile(hComm, buffer, sizeof(buffer), &bytesRead, NULL))
			{
				std::cout << "received: " << std::endl;
				
				for (int i = 0; i < bytesRead; i++)
				{
					int value = convertTwosComplementToInt(std::bitset<8>(buffer[i]));
					std::cout << value << std::endl;
				}
			}
	}
	
	int convertTwosComplementToInt(const std::bitset<8> byte)
	{
		return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
	}
	
	std::string portName;
	HANDLE hComm;
	std::thread workerThread;
	bool stopThread;
};



int main()
{
	COMPort comPort("COM3");
	
	if (comPort.open())
	{
		comPort.send("1");
		std::this_thread::sleep_for(std::chrono::seconds(10));
		comPort.send("0");
		comPort.close();
	}
	
	std::cin.get();
	return 0;
}
