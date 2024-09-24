#include "histogram.h"
#include <iostream>
#include <algorithm>

// Public Functions

Histogram::Histogram(Range<float> xScreen, Range<float> yScreen)
{
	array = new float[2 * maxSize];
	this->xScreen = xScreen;
	this->yScreen = yScreen;
}

Histogram::~Histogram()
{
	delete[] array;
}

void Histogram::setWindow(HWND hwnd)
{
	RECT rect;
	
	if(::GetWindowRect(hwnd, &rect))
	{
		screenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
	}
	
	xRange = Range<float>(0, screenWidth);
	yRange = Range<float>(0, screenHeight);
}

float* Histogram::data()
{
	return array;
}

int Histogram::size()
{
	return maxSize;
}

void Histogram::update(const std::vector<float>& data)
{
	float dx = screenWidth / data.size();
	float max = *std::max_element(data.begin(), data.end());
	
	array[0] = xRange.convertValueToNewRange(0, xScreen);
	array[1] = yRange.convertValueToNewRange(0, yScreen);
	
	Range<float> f = Range<float>(0, max + 100);
	
	for (int i = 0; i < data.size(); i++)
		addItem(i, dx, f.convertValueToNewRange(data[i], yScreen));
	
	array[2 * maxSize - 2] = xRange.convertValueToNewRange(0, xScreen);
	array[2 * maxSize - 1] = yRange.convertValueToNewRange(0, yScreen);
};

// Private Functions

void Histogram::addItem(const int index, const float dx, const float y)
{
	array[6 * index + 0] = xRange.convertValueToNewRange(index * dx, xScreen);
	array[6 * index + 1] = y;
	
	array[6 * index + 2] = xRange.convertValueToNewRange((index + 1) * dx, xScreen);
	array[6 * index + 3] = y;
	
	array[6 * index + 4] = xRange.convertValueToNewRange((index + 1) * dx, xScreen);
	array[6 * index + 5] = yRange.convertValueToNewRange(0, yScreen);
}
