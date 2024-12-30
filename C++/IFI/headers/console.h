#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <vector>
#include "text.h"

class Console
{
public:
	static Console& shared();
	void addWindow(Text& text);
	void print(const std::string& message, int windowIndex = 0);
	
private:
	Console() {}
   ~Console() {}
	
	std::vector<Text*> textWindows;
};

#endif // CONSOLE_H_INCLUDED
