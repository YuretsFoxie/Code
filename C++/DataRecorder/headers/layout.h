#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED

#include <windows.h>

using namespace std;

enum HAlignment {left, hCenter, right};
enum VAlignment {top,  vCenter, bottom};

class Layout
{
public:
	Layout(double widthProportion = 1, 
		   double heightProportion = 1,
		   HAlignment hAlignment = HAlignment::left, 
		   VAlignment vAlignment = VAlignment::top,
		   double hShiftProportion = 0,
		   double vShiftProportion = 0):
		   
		   widthProportion(widthProportion),
		   heightProportion(heightProportion),
		   hAlignment(hAlignment), 
		   vAlignment(vAlignment),
		   hShiftProportion(hShiftProportion),
		   vShiftProportion(vShiftProportion) {}
	
	RECT calculateRect(const RECT& window);
	
private:
	double widthProportion;
	double heightProportion;
	double hShiftProportion;
	double vShiftProportion;
	HAlignment hAlignment;
	VAlignment vAlignment;
};

#endif // LAYOUT_H_INCLUDED
