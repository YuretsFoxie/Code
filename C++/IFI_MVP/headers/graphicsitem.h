#ifndef GRAPHICSITEM_H_INCLUDED
#define GRAPHICSITEM_H_INCLUDED

#include "range.h"
#include <vector>

class GraphicsItem
{
public:
	GraphicsItem() {};
	virtual ~GraphicsItem() {};
	
	virtual float* data() = 0;
	virtual int pointsNumber() = 0;
	
	virtual void push(const float value) {}
	
protected:
	struct Point
	{
		float x;
		float y;
	};
};

#endif // GRAPHICSITEM_H_INCLUDED
