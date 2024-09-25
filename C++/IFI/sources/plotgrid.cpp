#include "plotgrid.h"

// Public Functions

PlotGrid::PlotGrid(Range<float> xRange, Range<float> yRange): xRange(xRange), yRange(yRange)
{
	array = new float[2 * size];
	calculatePoints();
}

PlotGrid::~PlotGrid()
{
	delete[] array;
}

float* PlotGrid::data()
{	
	return array;
}

int PlotGrid::pointsNumber()
{
	return size;
}

bool PlotGrid::isPushable()
{
	return false;
}

bool PlotGrid::isFFTUpdatable()
{
	return false;
}

// Private Functions

void PlotGrid::calculatePoints()
{
	array[0] = xRange.getMin();
	array[1] = yRange.getMin();
	
	array[2] = xRange.getMax();
	array[3] = yRange.getMin();
	
	array[4] = xRange.getMax();
	array[5] = yRange.getMax();
	
	array[6] = xRange.getMin();
	array[7] = yRange.getMax();
	
	array[8] = xRange.getMin();
	array[9] = yRange.getMin();
}
