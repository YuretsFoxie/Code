#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "settings.h"
#include "shaders.h"
#include "openglbuffer.h"
#include "openglcontext.h"

class Graphics
{
public:
	void initialize(HWND hwnd, const Settings& settings);
	void drawVertices(const std::vector<float>& vertices);
	void drawText(const std::string& text, float x, float y, float scale, float color[3]);
	
private:
	Shaders shaders = Shaders();
	OpenGLBuffer buffer;
	OpenGLContext context;
};

#endif // GRAPHICS_H_INCLUDED
