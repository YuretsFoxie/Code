#include "console.h"

// Public Functions

Console& Console::shared()
{
	static Console instance;
	return instance;
}

void Console::addWindow(Text& text)
{
	textWindows.push_back(&text);
}

void Console::print(const std::string& message, int windowIndex)
{	
	if (windowIndex < textWindows.size())
		textWindows[windowIndex]->print(message);
}
