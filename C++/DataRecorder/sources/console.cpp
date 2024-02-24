#include "console.h"

Console::Console(const HDC& hDC, const Frame& f): hDC(hDC), frame(f.getFrame())
{
	int lineHeight = ::DrawText(hDC, "F", 1, &frame, DT_END_ELLIPSIS | DT_EDITCONTROL | DT_CALCRECT);
	maxLinesNumber = (frame.bottom - frame.top) / lineHeight;
}

void Console::print(const int value)
{
	print(to_string(value));
}

void Console::print(const string& message)
{
	string str = to_string(lines.size() + 1) + "> " + message;
	lines.push_back(str);
}

void Console::render()
{
	string buffer;
	int size = lines.size();
	int i = (size > maxLinesNumber) ? size - maxLinesNumber : 0;
	
	for (; i < size; i++)
		buffer += lines[i] + "\n";
	
	::DrawText(hDC, buffer.c_str(), buffer.length(), &frame, DT_END_ELLIPSIS | DT_EDITCONTROL);
}
