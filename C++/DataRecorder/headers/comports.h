#ifndef COMPORTS_H_INCLUDED
#define COMPORTS_H_INCLUDED

#include <windows.h>
#include <string>
#include <bitset>
#include "observablemodel.h"
#include "comsettings.h"

using namespace std;

class COMPorts: public ObservableModel
{
public:
	COMPorts(): isReceiving(false) {}
    ~COMPorts();
	
	void selectPort(const COMSettingsData& data);
    void transmit(const string& message);
	void disconnect();
	bool isConnected();
	
private:
	void initPort();
	void deinitPort();
	void startReceiving();
	void stopReceiving();
	
	static DWORD WINAPI staticReceive(void* Param);
	DWORD receive();
	
	int convertTwosComplementToInt(const bitset<8> byte);
	
	// Variables
	
	static const int bufferSize = 64;
	const int timeout = 1000;
	
	bool isReceiving = false;
	uint8_t buffer[bufferSize];
	DWORD baudrate;
	HANDLE portHandle;
	HANDLE threadHandle;
};

#endif // COMPORTS_H_INCLUDED
