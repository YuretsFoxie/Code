#ifndef PLOTGRID_H_INCLUDED
#define PLOTGRID_H_INCLUDED

#include "graphicsitem.h"

class PlotGrid: public GraphicsItem
{
public:
	PlotGrid(Range<float> xRange, Range<float> yRange);
   ~PlotGrid();
	
	float* data();
	int pointsNumber();
	bool isPushable();
	bool isFFTUpdatable();
	
private:
	void calculatePoints();
	
	const int size = 5;
	const Range<float> xRange;
	const Range<float> yRange;
	
	float* array;
};

#endif // PLOTGRID_H_INCLUDED
