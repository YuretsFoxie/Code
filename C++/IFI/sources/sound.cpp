#include "sound.h"

void Sound::playTest()
{
	::PlaySound("C:\\Data\\Code\\C++\\IFI\\sounds\\blip1.wav", NULL, SND_ASYNC);
}

void Sound::playTestLoop()
{
	::PlaySound("C:\\Data\\Code\\C++\\IFI\\sounds\\blip1.wav", NULL, SND_ASYNC | SND_LOOP);
}

void Sound::stop()
{
	::PlaySound(NULL, NULL, 0);
}
