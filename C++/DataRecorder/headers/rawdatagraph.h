#ifndef RAWDATAGRAPH_H_INCLUDED
#define RAWDATAGRAPH_H_INCLUDED

#include <windows.h>
#include <vector>
#include <deque>
#include "graphinterface.h"
#include "frame.h"

using namespace std;

class RawDataGraph: public GraphInterface
{
public:
	RawDataGraph(const HDC& hDC, const Frame& f);
	void addValue(const int value);
	void print(const int value) {}
	void print(const string& message) {}
	void render();
	
private:
	int calculateDx(const int index);
	int calculateScreenY(const int value);
	void drawBackground();
	void drawBuffer();
	
	const double xPointsCount = 128.0; // 128 x scale, one second is 1/scale of the screen width, there are 128 values per second
	
	double dx;
	double yZero;
	double yPoints;
	
	HDC hDC;
	RECT frame;
	deque<POINT> buffer;
	vector<POINT> screenBuffer;
};

#endif // RAWDATAGRAPH_H_INCLUDED
