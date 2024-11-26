#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <windows.h>
#include <mmsystem.h>

class Sound
{
public:
    static Sound& shared()
    {
        static Sound instance;
        return instance;
    }
	
	void playTest();
	void playTestLoop();
	void stop();
	
private:
	Sound() {}
   ~Sound() {}
};

#endif // SOUND_H_INCLUDED
