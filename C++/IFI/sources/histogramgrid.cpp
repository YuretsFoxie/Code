#include "histogramgrid.h"

// Public Functions

HistogramGrid::HistogramGrid(Range<float> xRange, Range<float> yRange): xRange(xRange), yRange(yRange)
{
	array = new float[2 * size];
	dx = xRange.range() / (float)(Settings::shared().getData().spectrumN / 2);
	halfDx = dx / 2;
	calculatePoints();
}

HistogramGrid::~HistogramGrid()
{
	delete[] array;
}

float* HistogramGrid::data()
{	
	return array;
}

int HistogramGrid::pointsNumber()
{
	return size;
}

bool HistogramGrid::isPushable()
{
	return false;
}

bool HistogramGrid::isFFTUpdatable()
{
	return false;
}

// Private Functions

void HistogramGrid::calculatePoints()
{
	int dataSize = Settings::shared().getData().spectrumN / 2;
	
	for (int i = 0; i < dataSize; i++)
		addItem(i);
	
	array[0] = xRange.getMin();
	array[1] = yRange.getMin();
}

void HistogramGrid::addItem(const int index)
{
	array[6 * index + 0] = xRange.getMin() + index * dx + halfDx;
	array[6 * index + 1] = yRange.getMin();
	
	array[6 * index + 2] = xRange.getMin() + (index + 1) * dx + halfDx;
	array[6 * index + 3] = yRange.getMin();
	
	array[6 * index + 4] = xRange.getMin() + (index + 1) * dx + halfDx;
	array[6 * index + 5] = yRange.getMin() + ((index + 1) % 5 == 0 ? 2 * y : y);
}
