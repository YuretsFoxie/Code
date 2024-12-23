#ifndef GRAPHSUBWINDOW_H_INCLUDED
#define GRAPHSUBWINDOW_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "viewportparameters.h"

class Plot
{
public:
	Plot(const ViewPortParameters& parameters): parameters(parameters) {}
   ~Plot();
	
	void initialize(int maxPoints, int maxADCValue);
	void draw(const std::vector<float>& buffer);
	
private:
	void prepare(std::vector<float>& vertices, const std::vector<float>& buffer);
	void generateBuffers();
	void bindBuffers(int maxPoints);
	void configureVertexAttribPointer();
	void unbindBuffers();
	
	ViewPortParameters parameters;
	GLuint VBO, VAO;
	int maxPoints;
	int maxADCValue;
};

#endif // GRAPHSUBWINDOW_H_INCLUDED
