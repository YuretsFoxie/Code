#ifndef PLOT_H_INCLUDED
#define PLOT_H_INCLUDED

#include "graphicsitem.h"
#include <deque>
#include <algorithm>

class Plot: public GraphicsItem
{
public:
	Plot(const int size, Range<float> xScreen = Range<float>(-1, 1), Range<float> yScreen = Range<float>(-1, 1));
   ~Plot();
	
	void setWindow(HWND hwnd);
	void push(const float value);
	float* data();
	int size();
	
private:
	Range<float> xRange;
	Range<float> yRange;
	Range<float> xScreen;
	Range<float> yScreen;
	
	const int maxY = 128;
	const int minY = -127;
	const int maxSize;
	
	float screenWidth, screenHeight;
	float dx;
	
	std::deque<Point> deque;
	float* array;
};

#endif // PLOT_H_INCLUDED
