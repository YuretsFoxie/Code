#include "plotgrid.h"

// Public Functions

PlotGrid::PlotGrid(Range<float> xScreen, Range<float> yScreen)
{
	array = new float[2 * maxSize];
	this->xScreen = xScreen;
	this->yScreen = yScreen;
}

PlotGrid::~PlotGrid()
{
	delete[] array;
}

void PlotGrid::setWindow(HWND hwnd)
{
	RECT rect;
	
	if(::GetWindowRect(hwnd, &rect))
	{
		screenWidth = rect.right - rect.left;
		screenHeight = rect.bottom - rect.top;
	}
	
	xRange = Range<float>(0, screenWidth);
	yRange = Range<float>(0, screenHeight);
	
	calculatePoints();
}

float* PlotGrid::data()
{	
	return array;
}

int PlotGrid::size()
{
	return maxSize;
}

// Private Functions

void PlotGrid::calculatePoints()
{
	array[0] = xRange.convertValueToNewRange(0.0, xScreen);
	array[1] = yRange.convertValueToNewRange(0.0, yScreen);
	
	array[2] = xRange.convertValueToNewRange(screenWidth, xScreen);
	array[3] = yRange.convertValueToNewRange(0.0, yScreen);
	
	array[4] = xRange.convertValueToNewRange(screenWidth, xScreen);
	array[5] = yRange.convertValueToNewRange(screenHeight, yScreen);
	
	array[6] = xRange.convertValueToNewRange(0.0, xScreen);
	array[7] = yRange.convertValueToNewRange(screenHeight, yScreen);
	
	array[8] = xRange.convertValueToNewRange(0.0, xScreen);
	array[9] = yRange.convertValueToNewRange(0.0, yScreen);
}
