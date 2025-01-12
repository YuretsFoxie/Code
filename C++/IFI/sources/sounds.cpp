#include "sounds.h"

void Sounds::playHello()
{
	const char* path = "sounds\\hello.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playError()
{
	const char* path = "sounds\\error.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playButton()
{
	const char* path = "sounds\\button3.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playDisabled()
{
	const char* path = "sounds\\button10.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC);
}

void Sounds::playWarningLoop()
{
	const char* path = "sounds\\warn1.wav";
	::PlaySound(TEXT(path), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void Sounds::stop()
{
	::PlaySound(NULL, NULL, 0);
}
