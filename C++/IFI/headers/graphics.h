#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "settings.h"
#include "shaders.h"
#include "graphsubwindow.h"
#include "textsubwindow.h"

class Graphics
{
public:
	void initialize(HWND hwnd, const Settings& settings);
	void drawVertices(const std::vector<float>& vertices);
	void drawText(const std::string& message, float x, float y);
	
private:
	void setupPixelFormat(HWND hwnd);
	
	const float ortho[16] = 
	{
		2.0f / 1024, 0, 0, 0,
		0, 2.0f / 768, 0, 0,
		0, 0, -1.0f, 0,
		-1.0f, -1.0f, 0, 1.0f
	};
	
	Shaders shaders;
	GraphSubwindow graph;
	TextSubwindow text;
};

#endif // GRAPHICS_H_INCLUDED
