#include "plot.h"

// Public Functions

Plot::Plot(const int size, Range<float> xScreen, Range<float> yScreen): maxSize(size)
{
	array = new float[2 * maxSize];
	this->xScreen = xScreen;
	this->yScreen = yScreen;
}

Plot::~Plot()
{
	delete[] array;
}

void Plot::setWindow(HWND hwnd)
{
	RECT rect;
	
	if(::GetWindowRect(hwnd, &rect))
	{
		screenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
	}
	
	xRange = Range<float>(0, screenWidth);
	yRange = Range<float>(minY, maxY);
	dx = screenWidth / (maxSize - 1);
}

float* Plot::data()
{
	return array;
}

int Plot::size()
{
	return deque.size();
}

void Plot::push(const float value)
{
	float y = yRange.convertValueToNewRange(value, yScreen);
	deque.push_back({screenWidth, y});
	
	if (deque.size() > maxSize)
		deque.pop_front();
	
	for (int i = 0; i < deque.size(); i++)
	{
		if (i < deque.size() - 1)
			deque[i].x -= dx;
		
		array[2 * i] = xRange.convertValueToNewRange(deque[i].x, xScreen);
		array[2 * i + 1] = deque[i].y;
	}
}
