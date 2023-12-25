#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <windows.h>
#include <vector>

using namespace std;

class Renderer
{
public:	
    static Renderer& shared()
    {
        static Renderer r;
        return r;
    }
	
	// Public Functions
	
	void onStart();
	void draw(HDC& hdc, RECT& window);
	void onStop();
	void clear();
	
private:
	HPEN greenPen;
	HDC hdc;
	RECT window;
	
	vector<POINT> buffer;
	
	double halfHeight;
	double scaleX;
	double dx;
	double dy;
	
	// Private Functions
	
    Renderer(): dx(0)
	{
		buffer.reserve(100000);
	}
	
	void prepareDrawing(HDC& hdc, RECT& window);
	void drawMarks();
	void drawLine(POINT from, POINT to);
	void fillBufferWithVoltage();
	void fillBufferWithTime();
	void drawBuffer();
};

#endif // RENDERER_H_INCLUDED
