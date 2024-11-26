#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <atomic>
#include "graphics.h"
#include "databuffer.h"

class Renderer
{
public:
	Renderer(Graphics& graphics, DataBuffer& buffer, int batchSize) : graphics(graphics), buffer(buffer), BATCH_SIZE(batchSize) {}
	void renderFrame(HDC hdc, int& updateCounter, std::atomic<bool>& isRunning);
	
private:
	void drawVertices();
	void drawText();
	
	Graphics& graphics;
	DataBuffer& buffer;
	const int BATCH_SIZE;
};

#endif // RENDERER_H_INCLUDED
