#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "graphicsitem.h"
#include "settings.h"
#include <deque>
#include <algorithm>

class Histogram: public GraphicsItem
{
public:
	Histogram(Range<float> xRange, Range<float> yRange);
   ~Histogram();
	
	float* data();
	int pointsNumber();
	bool isPushable();
	bool isFFTUpdatable();
	
	void updateFFT(const std::vector<float>& data);
	
private:
	void addItem(const int index, const float y);
	
	const int size = 3 * Settings::shared().getData().spectrumN / 2 + 2;
	const Range<float> xRange;
	const Range<float> yRange;
	
	float* array;
	float dx = 0;
};

#endif // HISTOGRAM_H_INCLUDED
