#include "graphsubwindow.h"

// Public Functions

GraphSubwindow::~GraphSubwindow()
{
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
}

void GraphSubwindow::initialize(int maxPoints, int scaleFactor)
{
	this->maxPoints = maxPoints;
	this->scaleFactor = scaleFactor;
	
	generateBuffers();
	bindBuffers(maxPoints);
	configureVertexAttribPointer();
	unbindBuffers();	
}

void GraphSubwindow::draw(const std::vector<float>& buffer)
{
	std::vector<float> vertices;
	prepare(vertices, buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));	
}

// Private Functions

void GraphSubwindow::prepare(std::vector<float>& vertices, const std::vector<float>& buffer)
{
	vertices.reserve(buffer.size() * 2);
	
	const float scale = 2.0f / maxPoints;
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		float x = (scale * i) - 1.0f;
		float y = buffer[i] / static_cast<float>(scaleFactor);
		vertices.push_back(x);
		vertices.push_back(y);
	}
}

void GraphSubwindow::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void GraphSubwindow::bindBuffers(int maxPoints)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
}

void GraphSubwindow::configureVertexAttribPointer()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void GraphSubwindow::unbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
