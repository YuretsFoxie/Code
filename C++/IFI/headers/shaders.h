#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <GL/glew.h>

class Shaders
{
public:
	Shaders(): program(0) {}
   ~Shaders();
	
	void initialize();
	GLuint getProgram() const;
	
private:
	GLuint createVertexShader();
	GLuint createFragmentShader();
	void createProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint compileShader(GLenum type, const char* source);
	
	GLuint program;
};

#endif // SHADERS_H_INCLUDED
