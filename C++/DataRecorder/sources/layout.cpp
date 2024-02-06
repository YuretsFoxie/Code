#include "layout.h"

RECT Layout::calculateRect(const RECT& window)
{
	RECT result;
	
	int width = window.right * widthProportion;
	int height = window.bottom * heightProportion;
	
	result.left = window.right * hShiftProportion;
	result.top = window.bottom * vShiftProportion;
	
	if (hAlignment == hCenter) result.left += window.right * 0.5 - width * 0.5;
	if (hAlignment == right) result.left += window.right - width;
	
	if (vAlignment == vCenter) result.top += window.bottom * 0.5 - height * 0.5;
	if (vAlignment == bottom) result.top += window.bottom - height;
	
	result.right = result.left + width;
	result.bottom = result.top + height;
	
	return result;
}
