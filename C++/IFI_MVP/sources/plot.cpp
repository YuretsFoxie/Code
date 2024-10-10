#include "plot.h"

// Public Functions

Plot::Plot(Range<float> xRange, Range<float> yRange, const int size): xRange(xRange), yRange(yRange), size(size)
{
	array = new float[2 * size];
	dx = xRange.range() / (size - 1);
}

Plot::~Plot()
{
	delete[] array;
}

float* Plot::data()
{
	return array;
}

int Plot::pointsNumber()
{
	return deque.size();
}

void Plot::push(const float value)
{
	float y = amplitudeRange.convertValueToNewRange(value, yRange);
	deque.push_back({xRange.getMax(), y});
	
	if (deque.size() > size)
		deque.pop_front();
	
	for (int i = 0; i < deque.size(); i++)
	{
		if (i < deque.size() - 1)
			deque[i].x -= dx;
		
		array[2 * i] = deque[i].x;
		array[2 * i + 1] = deque[i].y;
	}
}
