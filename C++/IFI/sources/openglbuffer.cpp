#include "openglbuffer.h"

// Public Functins

OpenGLBuffer::~OpenGLBuffer()
{
	cleanup();
}

void OpenGLBuffer::prepare(int maxPoints)
{
	generateBuffers();
	bindBuffers(maxPoints);
	configureVertexAttribPointer();
	unbindBuffers();
}

void OpenGLBuffer::draw(const std::vector<float>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));
}

// Private Functions

void OpenGLBuffer::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void OpenGLBuffer::bindBuffers(int maxPoints)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
}

void OpenGLBuffer::configureVertexAttribPointer()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void OpenGLBuffer::unbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenGLBuffer::cleanup()
{
	if (VBO)
		glDeleteBuffers(1, &VBO);
		
	if (VAO)
		glDeleteVertexArrays(1, &VAO);
}
