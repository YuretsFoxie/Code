#include "sound.h"

void Sound::playButton()
{
	const char* path = "sounds\\blip1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sound::playError()
{
	const char* path = "sounds\\warn1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sound::playTestLoop()
{
	const char* path = "sounds\\warn1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void Sound::stop()
{
	::PlaySound(NULL, NULL, 0);
}
