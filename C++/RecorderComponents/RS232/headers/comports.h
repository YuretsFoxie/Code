#ifndef COMPORTS_H_INCLUDED
#define COMPORTS_H_INCLUDED

#include <windows.h>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

class COMPorts
{
public:
	static COMPorts& shared()
    {
        static COMPorts ports;
        return ports;
	}
	
	vector<string> findPorts();
	void selectPort(const string& port);
	void disconnect();
	
    void transmit(const string& data);
    void startReceiving();
    void stopReceiving();
	
private:
	COMPorts(): isReceiving(false), timeout(1000), baudrate(CBR_115200) {} // select the baudrate correctly on both sides, see the link, the max speeds are not supported on this machine
    ~COMPorts();
	void initPort();
	void deinitPort();
    static DWORD WINAPI staticReceive(void* Param);
    DWORD receive();
	void saveData();
	int convertTwosComplementToInt(const bitset<8> byte);
	
	// Variables
	
    bool isReceiving;
    int timeout;
	uint8_t buffer[64];
	
	DWORD baudrate;
	DWORD bytesRead;
    HANDLE portHandle;
    HANDLE threadHandle;
};

#endif // COMPORTS_H_INCLUDED
