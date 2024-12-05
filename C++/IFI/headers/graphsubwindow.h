#ifndef GRAPHSUBWINDOW_H_INCLUDED
#define GRAPHSUBWINDOW_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "subwindow.h"

class GraphSubwindow: public Subwindow
{
public:
	// void render() override;
	
	GraphSubwindow(const Origin& origin = {0}, const Size& size = {0}): Subwindow(origin, size) {}
   ~GraphSubwindow() override;
	
	void initialize(int maxPoints);
	void draw(const std::vector<float>& vertices);
	
private:
	void generateBuffers();
	void bindBuffers(int maxPoints);
	void configureVertexAttribPointer();
	void unbindBuffers();
	void cleanup();
	
	GLuint VBO, VAO;
};

#endif // GRAPHSUBWINDOW_H_INCLUDED
