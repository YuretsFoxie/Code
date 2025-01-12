#include "shaders.h"

// Public Functins

Shaders::Shaders()
{
	GLuint graphVertexShader = compileShader(GL_VERTEX_SHADER, graphVertexShaderSource);
	GLuint graphFragmentShader = compileShader(GL_FRAGMENT_SHADER, graphFragmentShaderSource);	
	GLuint textVertexShader = compileShader(GL_VERTEX_SHADER, textVertexShaderSource);
	GLuint textFragmentShader = compileShader(GL_FRAGMENT_SHADER, textFragmentShaderSource);
	
	graphProgram = linkProgram(graphVertexShader, graphFragmentShader);
	textProgram = linkProgram(textVertexShader, textFragmentShader);
	
	glDeleteShader(graphVertexShader);
	glDeleteShader(graphFragmentShader);
	glDeleteShader(textVertexShader);
	glDeleteShader(textFragmentShader);	
}

Shaders::~Shaders()
{
	if (graphProgram)
		glDeleteProgram(graphProgram);
	
	if (textProgram)
		glDeleteProgram(textProgram);
}

GLuint Shaders::getGraphProgram() const
{
	return graphProgram;
}

GLuint Shaders::getTextProgram() const
{
	return textProgram;
}

// Private Functions

GLuint Shaders::compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint Shaders::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	return program;
}
