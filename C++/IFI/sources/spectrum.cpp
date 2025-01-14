#include <algorithm>
#include "spectrum.h"

// Public Functions

Spectrum::Spectrum(Settings& settings, SpectrumAnalyzer& analyzer, const ViewPortParameters& parameters):
	pointsCount(3 * settings.getBufferSize() / 2 + 2),
	analyzer(analyzer),
	parameters(parameters)
{
	array = new float[2 * pointsCount];
	dx = 2.0 / settings.getBufferSize();
}

Spectrum::~Spectrum()
{
	if (VAO)
	{	
		glDeleteVertexArrays(1, &VAO);
	}
	
	if (VBO)
	{
		glDeleteBuffers(1, &VBO);
	}
	
	delete[] array;
}

void Spectrum::setup()
{
	generateBuffers();
	bindBuffers();
	configureVertexAttribPointer();
	unbindBuffers();	
}

void Spectrum::draw()
{
	glViewport(parameters.x, parameters.y, parameters.width, parameters.height);
	updateFFT();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * pointsCount * sizeof(float), array);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(pointsCount));
}

// Private Functions

void Spectrum::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void Spectrum::bindBuffers()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointsCount * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
}

void Spectrum::configureVertexAttribPointer()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Spectrum::unbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Spectrum::updateFFT()
{
	const std::vector<float> data = analyzer.getData();
	float max = *std::max_element(data.begin(), data.end());
	
	array[0] = -1.0;
	array[1] = -1.0;
	array[2 * pointsCount - 2] = -1.0;
	array[2 * pointsCount - 1] = -1.0;
	
	for (int i = 0; i < data.size(); i++)
	{
		addItem(i, convert(data[i], max));
	}
};

float Spectrum::convert(const float value, const float max)
{
	return ((value * 2) / max) - 1;
}

void Spectrum::addItem(const int index, const float y)
{
	array[6 * index + 0] = -1.0 + index * dx;
	array[6 * index + 1] = y;
	
	array[6 * index + 2] = -1.0 + (index + 1) * dx;
	array[6 * index + 3] = y;
	
	array[6 * index + 4] = -1.0 + (index + 1) * dx;
	array[6 * index + 5] = -1.0;
}
