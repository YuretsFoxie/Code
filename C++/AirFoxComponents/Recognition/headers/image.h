#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

using namespace std;

struct Color
{
	int getGrayscaleValue()
	{
		return (red + green + blue) / 3;
	}
	
	int red;
	int green;
	int blue;
};



class Image
{
public:
	Image(const string& name);
	
	vector<int> getData();
	int getWidth();
	int getHeight();
	
private:
	void load();
	
	string name;
	BITMAP bitmap;
};

#endif // IMAGE_H_INCLUDED
