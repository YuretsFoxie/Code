#include "histogram.h"
#include <algorithm>

// Public Functions

Histogram::Histogram(Range<float> xRange, Range<float> yRange): xRange(xRange), yRange(yRange)
{
	array = new float[2 * size];
	dx = xRange.range() / (float)(Settings::shared().getData().spectrumN / 2);
}

Histogram::~Histogram()
{
	delete[] array;
}

float* Histogram::data()
{
	return array;
}

int Histogram::pointsNumber()
{
	return size;
}

bool Histogram::isPushable()
{
	return false;
}

bool Histogram::isFFTUpdatable()
{
	return true;
}

void Histogram::updateFFT(const std::vector<float>& data)
{
	float max = *std::max_element(data.begin(), data.end());
	
	array[0] = xRange.getMin();
	array[1] = yRange.getMin();
	array[2 * size - 2] = xRange.getMin();
	array[2 * size - 1] = yRange.getMin();
	
	for (int i = 0; i < data.size(); i++)
	{
		float y = Range<float>(0, max + 100).convertValueToNewRange(data[i], yRange);
		addItem(i, y);
	}
};

// Private Functions

void Histogram::addItem(const int index, const float y)
{
	array[6 * index + 0] = xRange.getMin() + index * dx;
	array[6 * index + 1] = y;
	
	array[6 * index + 2] = xRange.getMin() + (index + 1) * dx;
	array[6 * index + 3] = y;
	
	array[6 * index + 4] = xRange.getMin() + (index + 1) * dx;
	array[6 * index + 5] = yRange.getMin();
}
