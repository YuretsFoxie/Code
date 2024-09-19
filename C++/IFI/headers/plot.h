#ifndef PLOT_H_INCLUDED
#define PLOT_H_INCLUDED

#include "range.h"
#include <deque>
#include <algorithm>
#include <windows.h>

class Plot
{
public:
	Plot(const int size);
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
	Range<float> screen;

	float screenWidth, screenHeight;
	float dx;
	
	const int size;
	std::deque<Point> deque;
	float* array;
};

#endif // PLOT_H_INCLUDED
