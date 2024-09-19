#ifndef PLOT_H_INCLUDED
#define PLOT_H_INCLUDED

#include "range.h"
#include <deque>
#include <algorithm>
#include <windows.h>

class Plot
{
public:
	Plot(const int size, Range<float> xScreen = Range<float>(-1, 1), Range<float> yScreen = Range<float>(-1, 1));
   ~Plot();
	
	void setWindow(HWND hwnd);
	void push(const float value);
	float* data();
	int currentSize();
	int maxSize();
	
private:
	struct Point
	{
		float x;
		float y;
	};
	
	Range<float> xRange;
	Range<float> yRange;
	Range<float> xScreen;
	Range<float> yScreen;
	
	float screenWidth, screenHeight;
	float dx;
	
	const int size;
	std::deque<Point> deque;
	float* array;
};

#endif // PLOT_H_INCLUDED
