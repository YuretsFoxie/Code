#ifndef SPECTRUM_H_INCLUDED
#define SPECTRUM_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "settings.h"
#include "spectrumanalyzer.h"
#include "viewportparameters.h"

struct Spectrum
{
public:
	Spectrum(Settings& settings, SpectrumAnalyzer& analyzer, const ViewPortParameters& parameters);
   ~Spectrum();
	
	void setup();
	void draw();
	
private:
	void generateBuffers();
	void bindBuffers();
	void configureVertexAttribPointer();
	void unbindBuffers();
	void updateFFT(const std::vector<float>& data);
	float convert(const float value, const float max);
	void addItem(const int index, const float y);
	
	const ViewPortParameters& parameters;
	SpectrumAnalyzer& analyzer;
	
	GLuint VBO, VAO;
	float* array;
	float dx = 0;
	int pointsCount;
};

#endif // SPECTRUM_H_INCLUDED
