#ifndef PLOTGRID_H_INCLUDED
#define PLOTGRID_H_INCLUDED

#include "graphicsitem.h"

class PlotGrid: public GraphicsItem
{
public:
	PlotGrid(Range<float> xScreen = Range<float>(-1, 1), Range<float> yScreen = Range<float>(-1, 1));
   ~PlotGrid();
	
	void setWindow(HWND hwnd);
	float* data();
	int size();
	
private:
	void calculatePoints();
	
	Range<float> xRange;
	Range<float> yRange;
	Range<float> xScreen;
	Range<float> yScreen;
	
	const int maxSize = 5;
	
	float screenWidth, screenHeight;
	float* array;
};

#endif // PLOTGRID_H_INCLUDED
