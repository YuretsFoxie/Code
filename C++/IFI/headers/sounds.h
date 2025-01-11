#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <windows.h>
#include <mmsystem.h>

class Sounds
{
public:	
	void playButton();
	void playError();
	void playTestLoop();
	void stop();
};

#endif // SOUNDS_H_INCLUDED
