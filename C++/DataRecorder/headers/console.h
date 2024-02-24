#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <vector>
#include <string>
#include "graphinterface.h"
#include "frame.h"

using namespace std;

class Console: public GraphInterface
{
public:
	Console(const HDC& hDC, const Frame& f);
	void addValue(const int value) {}
	void print(const int value);
	void print(const string& message);
	void render();
	
private:
	vector<string> lines;
	HDC hDC;
	RECT frame;
	int maxLinesNumber;
};

#endif // CONSOLE_H_INCLUDED
