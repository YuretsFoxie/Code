#include "plot.h"
#include <iostream>

Plot::Plot(const int size): size(size)
{
	array = new float[2 * size];
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
	yRange = Range<float>(-127, 128);
	screen = Range<float>(-1, 1);
	dx = screenWidth / (maxSize() - 1);
}

void Plot::push(const float value)
{
	float y = yRange.convertValueToNewRange(value, screen);
	deque.push_back({screenWidth, y});
	
	if (deque.size() > size)
		deque.pop_front();
	
	for (int i = 0; i < deque.size(); i++)
	{
		if (i < deque.size() - 1)
			deque[i].x -= dx;
		
		array[2 * i] = xRange.convertValueToNewRange(deque[i].x, screen);
		array[2 * i + 1] = deque[i].y;
	}
}

float* Plot::data()
{
	return array;
}

int Plot::currentSize()
{
	return deque.size();
}

int Plot::maxSize()
{
	return size;
}
