#include "renderer.h"

// Public Functins

void Renderer::renderFrame(HDC hdc, int& updateCounter, std::atomic<bool>& isRunning)
{
	updateCounter++;
	if (updateCounter >= BATCH_SIZE)
	{
		drawVertices();
		drawText();
		::SwapBuffers(hdc);
		updateCounter = 0;
	}
}

// Private Functions

void Renderer::drawVertices()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<float> vertices;
	buffer.prepare(vertices);
	
	if (!vertices.empty())
		graphics.drawVertices(vertices);
}

void Renderer::drawText()
{
	float color[3] = {0.0f, 1.0f, 0.0f};
	graphics.drawText("UART Data Visualization", 25.0f, 25.0f, 1.0f, color);
}
