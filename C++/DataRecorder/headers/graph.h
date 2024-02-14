#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <windows.h>
#include <vector>

using namespace std;

class GraphInterface
{
public:
	virtual ~GraphInterface() {};
	virtual void addValue(const int value) = 0;
	virtual void render() = 0;
};



class SoundGraph: public GraphInterface
{
public:
	SoundGraph() {}
	SoundGraph(const HDC& memDC, const RECT& frame);
	
	void addValue(const int value);
	void render();
	
private:
	HDC memDC;
	RECT frame;
	vector<POINT> buffer;
};

#endif // GRAPH_H_INCLUDED
