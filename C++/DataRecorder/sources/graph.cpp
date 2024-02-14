#include "graph.h"
#include "console.h"

SoundGraph::SoundGraph(const HDC& memDC, const RECT& frame): memDC(memDC), frame(frame)
{
	// buffer = {{0, 0}, {0, 200}, {200, 200}, {200, 0}, {0, 0}};	
	buffer = {{frame.left, frame.top}, {frame.right, frame.top}, {frame.right, frame.bottom}, {frame.left, frame.bottom}, {frame.left, frame.top}};
}

void SoundGraph::addValue(const int value)
{
	
}

void SoundGraph::render()
{
	::Polyline(memDC, &buffer[0], buffer.size());
}
