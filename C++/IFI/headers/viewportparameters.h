#ifndef VIEWPORTPARAMETERS_H_INCLUDED
#define VIEWPORTPARAMETERS_H_INCLUDED

#include "settings.h"

struct ViewPortParameters
{
	ViewPortParameters(const Settings& s, 
					   const float x = 0,
					   const float y = 0,
					   const float width = 1,
					   const float height = 1):
	
		x(x * s.getWindowWidth()), 
		y(y * s.getWindowHeight()),
		width(width * s.getWindowWidth()), 
		height(height * s.getWindowHeight()) {}
	
	const float x;			// viewport lower left corner x (left -> right)
	const float y;			// viewport lower left corner y (bottom -> top)
	const float width;
	const float height;
};

#endif // VIEWPORTPARAMETERS_H_INCLUDED
