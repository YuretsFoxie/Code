#ifndef GRAPHSUBWINDOW_H_INCLUDED
#define GRAPHSUBWINDOW_H_INCLUDED

#include <GL/glew.h>
#include <vector>
#include "subwindow.h"

class GraphSubwindow: public Subwindow
{
public:
	GraphSubwindow(const Origin& origin, const Size& size, const std::vector<float>& data): Subwindow(origin, size), data(data) {}
	void render() override;
	
private:
	std::vector<float> data;
	GLuint VBO, VAO;
};

#endif // GRAPHSUBWINDOW_H_INCLUDED
