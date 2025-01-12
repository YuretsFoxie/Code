#ifndef GRAPHSUBWINDOW_H_INCLUDED
#define GRAPHSUBWINDOW_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "viewportparameters.h"
#include "settings.h"
#include "databuffer.h"

class Plot
{
public:
	Plot(Settings& settings, DataBuffer& buffer, const ViewPortParameters& parameters);
   ~Plot();
	
	void draw();
	
private:
	void prepare(std::vector<float>& vertices, const std::vector<float>& buffer);
	void generateBuffers();
	void bindBuffers();
	void configureVertexAttribPointer();
	void unbindBuffers();
	
	const ViewPortParameters& parameters;
	DataBuffer& buffer;
	
	GLuint VBO, VAO;
	int bufferSize;
	int maxADCValue;
};

#endif // GRAPHSUBWINDOW_H_INCLUDED
