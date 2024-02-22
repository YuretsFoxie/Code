#include "graph.h"
#include "console.h"

RawDataGraph::RawDataGraph(const HDC& hDC, const Frame& f): hDC(hDC), frame(f.getFrame())
{
	screenBuffer.reserve(xPointsCount);
	dx = (frame.right - frame.left) / xPointsCount;
	
	int height = frame.bottom - frame.top;
	yZero = frame.top + height / 2;
	yPoints = height / 1024.0;
}

void RawDataGraph::addValue(const int value) // one second is 1/10 of the screen width, there are 128 values per second
{
	if (buffer.size() > xPointsCount)
		buffer.pop_front();
	
	buffer.push_back({0, calculateScreenY(value)});	
}

void RawDataGraph::render()
{
	drawBackground();
	drawBuffer();
}

int RawDataGraph::calculateDx(const int index)
{
	return frame.right - index * dx;
}

int RawDataGraph::calculateScreenY(const int value) // the value range is from -512 to +511
{
	return yZero - value * yPoints;
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
	screenBuffer.clear();
	
	for (int i = 0; i < buffer.size(); i++)
	{
		POINT p = { calculateDx( buffer.size() - i - 1 ), buffer[i].y };
		screenBuffer.push_back(p);
	}
	
	::Polyline(hDC, &screenBuffer[0], screenBuffer.size());
}
