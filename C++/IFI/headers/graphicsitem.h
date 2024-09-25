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
	virtual bool isPushable() = 0;
	virtual bool isFFTUpdatable() = 0;
	
	virtual void push(const float value) {}
	virtual void updateFFT(const std::vector<float>& data) {}
	
protected:
	struct Point
	{
		float x;
		float y;
	};
};

#endif // GRAPHICSITEM_H_INCLUDED
