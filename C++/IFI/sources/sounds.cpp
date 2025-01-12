#include "sounds.h"

void Sounds::playButton()
{
	const char* path = "sounds\\blip1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playDisabled()
{
	const char* path = "sounds\\button8.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playWarning()
{
	const char* path = "sounds\\warn1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playTestLoop()
{
	const char* path = "sounds\\warn1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void Sounds::stop()
{
	::PlaySound(NULL, NULL, 0);
}
