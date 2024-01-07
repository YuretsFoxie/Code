#include <iostream>
#include "image.h"

Image::Image(const string& name): name(name)
{
	load();
}

vector<int> Image::getData()
{
	BYTE* buffer = (BYTE*)bitmap.bmBits;
	vector<int> colors;
	int bytes = bitmap.bmBitsPixel >> 3;
	
	for (int i = 0; i < bitmap.bmWidth * bitmap.bmHeight * bytes; i += bytes)
	{
		if (i % bytes == 0)
		{
			Color c;
			c.red = buffer[i];
			c.green = buffer[i + 1];
			c.blue = buffer[i + 2];
			
			colors.push_back( c.getGrayscaleValue() );
		}
	}
	
	return colors;
}

int Image::getWidth()
{
	return bitmap.bmWidth;
}

int Image::getHeight()
{
	return bitmap.bmHeight;
}

void Image::load()
{
	wstring wideName = wstring(name.begin(), name.end());
	wchar_t* filename = const_cast< wchar_t* >(wideName.c_str());
	
	HBITMAP hBitmap;
    Gdiplus::Bitmap b(filename, false);
	b.GetHBITMAP(0, &hBitmap);
	
	::GetObject(hBitmap, sizeof(bitmap), (LPVOID)&bitmap);
}
