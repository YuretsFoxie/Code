#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include "graphicsitem.h"
#include "settings.h"
#include <deque>
#include <algorithm>

class Histogram: public GraphicsItem
{
public:
	Histogram(Range<float> xScreen = Range<float>(-1, 1), Range<float> yScreen = Range<float>(-1, 1));
   ~Histogram();
	
	void setWindow(HWND hwnd);
	float* data();
	int size();
	void update(const std::vector<float>& data);
	
private:
	void addItem(const int index, const float dx, const float y);
	
	Range<float> xRange;
	Range<float> yRange;
	Range<float> xScreen;
	Range<float> yScreen;
	
	const int maxSize = 3 * Settings::shared().getData().spectrumN / 2 + 2;
	
	float screenWidth, screenHeight;
	float* array;
};

#endif // HISTOGRAM_H_INCLUDED
