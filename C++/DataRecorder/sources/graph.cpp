#include "graph.h"
#include "console.h"

RawDataGraph::RawDataGraph(const HDC& hDC, const Frame& f): hDC(hDC), frame(f.getFrame())
{
	
}

void RawDataGraph::addValue(const int value) // one second is 1/10 of the screen width, there are 128 values per second
{
	buffer.push_back({0, calculateScreenY(value)});
}

void RawDataGraph::render()
{
	drawBackground();
	drawBuffer();
}

int RawDataGraph::calculateDx(const int index, const int scale)
{
	int width = frame.right - frame.left;
	double dx = width / (128.0 * scale);
	return frame.right - index * dx;
}

int RawDataGraph::calculateScreenY(const int value) // the value range is from -512 to +511
{	
	int height = frame.bottom - frame.top;
	int zero = frame.top + height / 2;
	double shift = value * height / 1024.0;
	
	return zero - shift;
}

void RawDataGraph::drawBackground()
{
	::MoveToEx(hDC, frame.left, frame.top, NULL);
	::LineTo(hDC, frame.right - 1, frame.top);
	::LineTo(hDC, frame.right - 1, frame.bottom - 1);
	::LineTo(hDC, frame.left, frame.bottom - 1);
	::LineTo(hDC, frame.left, frame.top);
	
	int y = calculateScreenY(0);
	::MoveToEx(hDC, frame.left, y, NULL);
	::LineTo(hDC, frame.right, y);
}

void RawDataGraph::drawBuffer()
{
	vector<POINT> screenBuffer;
	
	for (int i = 0; i < buffer.size(); i++)
	{
		POINT p = { calculateDx( buffer.size() - i - 1 ), buffer[i].y};
		screenBuffer.push_back(p);
	}
	
	::Polyline(hDC, &screenBuffer[0], screenBuffer.size());
}
