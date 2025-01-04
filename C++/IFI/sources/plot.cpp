#include "plot.h"

// Public Functions

Plot::~Plot()
{
	if (VAO)
	{	
		glDeleteVertexArrays(1, &VAO);
	}
	
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
	}
}

void Plot::setup(const Settings& settings)
{
	bufferSize = settings.getBufferSize();
	maxADCValue =settings.getMaxADCValue();
	
	generateBuffers();
	bindBuffers();
	configureVertexAttribPointer();
	unbindBuffers();	
}

void Plot::draw(const std::vector<float>& buffer)
{
	std::vector<float> vertices;
	prepare(vertices, buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));	
}

// Private Functions

void Plot::prepare(std::vector<float>& vertices, const std::vector<float>& buffer)
{
	vertices.reserve(buffer.size() * 2);
	
	const float scale = 2.0f / bufferSize;
	for (size_t i = 0; i < buffer.size(); ++i)
	{
		float x = (scale * i) - 1.0f;
		float y = buffer[i] / maxADCValue;
		vertices.push_back(x);
		vertices.push_back(y);
	}
}

void Plot::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void Plot::bindBuffers()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
}

void Plot::configureVertexAttribPointer()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Plot::unbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
