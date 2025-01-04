#ifndef SPECTRUM_H_INCLUDED
#define SPECTRUM_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include "viewportparameters.h"
#include "settings.h"

struct Spectrum
{
public:
	Spectrum(const ViewPortParameters& parameters): parameters(parameters) {}
   ~Spectrum() {}
	
	void setup(const Settings& settings) {}
	void draw(const std::vector<float>& buffer) {}
	
private:
	// void prepare(std::vector<float>& vertices, const std::vector<float>& buffer);
	void generateBuffers() {}
	void bindBuffers(int maxPoints) {}
	void configureVertexAttribPointer() {}
	void unbindBuffers() {}
	
	ViewPortParameters parameters;
	GLuint VBO, VAO;
	int bufferSize;
};

#endif // SPECTRUM_H_INCLUDED
