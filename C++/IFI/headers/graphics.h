#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <atomic>
#include "settings.h"
#include "shaders.h"
#include "databuffer.h"
#include "graphsubwindow.h"
#include "textsubwindow.h"

class Graphics
{
public:
	Graphics(DataBuffer& buffer, Settings& settings): buffer(buffer), settings(settings) {}
	
	void set(const HWND& hwnd);
	void render(HDC hdc, std::atomic<bool>& isRunning);
	
private:
	void setupPixelFormat(HWND hwnd);
	void drawVertices();
	void drawText();
	
	const float ortho[16] = 
	{
		2.0f / 1024, 0, 0, 0,
		0, 2.0f / 768, 0, 0,
		0, 0, -1.0f, 0,
		-1.0f, -1.0f, 0, 1.0f
	};
	
	DataBuffer& buffer;
	Settings& settings;
	
	Shaders shaders;
	GraphSubwindow graph;
	TextSubwindow text;
	
	int batchSize;
	int updateCounter = 0;
};

#endif // GRAPHICS_H_INCLUDED
