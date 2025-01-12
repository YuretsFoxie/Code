#ifndef COMPORT_H_INCLUDED
#define COMPORT_H_INCLUDED

#include <windows.h>
#include <atomic>
#include "settings.h"
#include "databuffer.h"
#include "text.h"
#include "sounds.h"

class COMPort
{
public:
	COMPort();
	COMPort(Settings& settings, DataBuffer& buffer1, DataBuffer& buffer2, Text& text, Sounds& sounds);
   ~COMPort();
	
	void toggleTransmission();
	
private:
	void open();
	void close();
	void setup();
	void run();
	void read();
	bool isOpened();
	
	Settings& settings;
	DataBuffer& buffer1;
	DataBuffer& buffer2;
	Text& text;
	Sounds& sounds;
	
	std::atomic<bool> isReceiving;
	bool isPushedToBuffer1;
	HANDLE handle;
};

#endif // COMPORT_H_INCLUDED
