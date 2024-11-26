#ifndef HISTOGRAMGRID_H_INCLUDED
#define HISTOGRAMGRID_H_INCLUDED

#include "graphicsitem.h"
#include "settings.h"

class HistogramGrid: public GraphicsItem
{
public:
	HistogramGrid(Range<float> xRange, Range<float> yRange);
   ~HistogramGrid();
	
	float* data();
	int pointsNumber();
	bool isPushable();
	bool isFFTUpdatable();
	
private:
	void calculatePoints();
	void addItem(const int index);
	
	const int size = 3 * Settings::shared().getData().spectrumN / 2;
	const float y = 0.02;
	const Range<float> xRange;
	const Range<float> yRange;
	
	float* array;
	float dx = 0;
	float halfDx = 0;
};

#endif // HISTOGRAMGRID_H_INCLUDED
