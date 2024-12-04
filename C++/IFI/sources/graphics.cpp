#include "graphics.h"

// Public Functins

void Graphics::initialize(HWND hwnd, const Settings& settings)
{
	context.initialize(hwnd);
	shaders.initialize();
	buffer.prepare(settings.getMaxPoints());
}

void Graphics::drawVertices(const std::vector<float>& vertices)
{
	glUseProgram(shaders.getGraphProgram());
	buffer.draw(vertices);
}

void Graphics::drawText(const std::string& text, float x, float y)
{
	context.renderText(text, x, y);
}
