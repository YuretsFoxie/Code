#ifndef GRAPHICSITEM_H_INCLUDED
#define GRAPHICSITEM_H_INCLUDED

#include "range.h"
#include <windows.h>

class GraphicsItem
{
public:
	GraphicsItem() {};
	virtual ~GraphicsItem() {};
	
	virtual void setWindow(HWND hwnd) = 0;
	virtual void push(const float value) {};
	virtual float* data() = 0;
	virtual int size() = 0;
	
protected:
	struct Point
	{
		float x;
		float y;
	};
};

#endif // GRAPHICSITEM_H_INCLUDED
