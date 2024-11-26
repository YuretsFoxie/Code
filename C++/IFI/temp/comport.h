#ifndef COMPORT_H_INCLUDED
#define COMPORT_H_INCLUDED

#include <windows.h>
#include <string>
#include <thread>
#include <bitset>
#include <atomic>

class COMPort
{
public:
    static COMPort& shared()
	{
		static COMPort instance;
		return instance;
	}
	
	void toggle();
	
private:	
	COMPort();
   ~COMPort();
	
	void start();
	void stop();
	bool open();
	void close();
	void send(const std::string& data);
	void setup();
	void receice();
	int  convertTwosComplementToInt(const std::bitset<8> byte);
	
	HANDLE hComm = INVALID_HANDLE_VALUE;
	std::string portName = "COM3";
	std::thread workerThread;
	std::atomic<bool> isReceiving;
};

#endif // COMPORT_H_INCLUDED
