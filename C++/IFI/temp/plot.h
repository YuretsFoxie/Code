#ifndef PLOT_H_INCLUDED
#define PLOT_H_INCLUDED

#include "graphicsitem.h"
#include <deque>
#include <algorithm>

class Plot: public GraphicsItem
{
public:
	Plot(Range<float> xRange, Range<float> yRange, const int size);
   ~Plot();
	
	float* data();
	int pointsNumber();
	bool isPushable();
	bool isFFTUpdatable();
	
	void push(const float value);
	
private:
	const int size;
	const Range<float> xRange;
	const Range<float> yRange;
	
	Range<float> amplitudeRange = Range<float>(-128, 128);
	std::deque<Point> deque;
	float* array;
	float dx;
};

#endif // PLOT_H_INCLUDED
