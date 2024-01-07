#ifndef GPlUS_H_INCLUDED
#define GPlUS_H_INCLUDED

#include <windows.h>
#include <gdiplus.h>

using namespace std;

class GDIPlus
{
public:
	void start();
	void stop();
	
private:
	ULONG_PTR gdiplusToken;
};

#endif // GPlUS_H_INCLUDED
