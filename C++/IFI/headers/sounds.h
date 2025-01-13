#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <windows.h>
#include <mmsystem.h>

class Sounds
{
public:
	void playSuccess();
	void playError();
	void playButton();
	void playDisabled();
	void playQuit();
	void playWarningLoop();
	void stop();
};

#endif // SOUNDS_H_INCLUDED
