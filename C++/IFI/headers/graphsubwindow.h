#ifndef GRAPHSUBWINDOW_H_INCLUDED
#define GRAPHSUBWINDOW_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "subwindow.h"

class GraphSubwindow
{
public:
	GraphSubwindow(const Origin& origin = {0}, const Size& size = {0}): origin(origin), size(size) {}
   ~GraphSubwindow();
	
	void initialize(int maxPoints, int scaleFactor);
	void draw(const std::vector<float>& buffer);
	
private:
	void prepare(std::vector<float>& vertices, const std::vector<float>& buffer);
	void generateBuffers();
	void bindBuffers(int maxPoints);
	void configureVertexAttribPointer();
	void unbindBuffers();
	void cleanup();
	
	GLuint VBO, VAO;
	Origin origin;
	Size size;
	
	int maxPoints = 0;
	int scaleFactor = 0;
};

#endif // GRAPHSUBWINDOW_H_INCLUDED
