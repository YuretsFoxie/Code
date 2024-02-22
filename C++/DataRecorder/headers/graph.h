#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <windows.h>
#include <vector>
#include <deque>
#include "frame.h"

using namespace std;

class GraphInterface
{
public:
	virtual ~GraphInterface() {};
	virtual void addValue(const int value) = 0;
	virtual void render() = 0;
};



class RawDataGraph: public GraphInterface
{
public:
	RawDataGraph() {}
	RawDataGraph(const HDC& hDC, const Frame& f);
	
	void addValue(const int value);
	void render();
	
private:
	int calculateDx(const int index, const int scale = 1);
	int calculateScreenY(const int value);
	void drawBackground();
	void drawBuffer();
	
	HDC hDC;
	RECT frame;
	vector<POINT> buffer;
	int count = 1;
};

#endif // GRAPH_H_INCLUDED
