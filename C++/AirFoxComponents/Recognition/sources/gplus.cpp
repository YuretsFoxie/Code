#include "gplus.h"

void GDIPlus::start()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void GDIPlus::stop()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
