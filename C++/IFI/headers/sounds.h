#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <windows.h>
#include <mmsystem.h>

class Sounds
{
public:
	void playHello();
	void playError();
	void playButton();
	void playDisabled();
	void playWarningLoop();
	void stop();
};

#endif // SOUNDS_H_INCLUDED
