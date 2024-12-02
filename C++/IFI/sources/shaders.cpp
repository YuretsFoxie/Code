#include "shaders.h"

// Public Functins

Shaders::~Shaders()
{
	if (program) glDeleteProgram(program);
}

void Shaders::initialize()
{
	GLuint vertexShader = createVertexShader();
	GLuint fragmentShader = createFragmentShader();
	createProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//=====
	
	GLuint textVertexShader = compileShader(GL_VERTEX_SHADER, textVertexShaderSource);
	GLuint textFragmentShader = compileShader(GL_FRAGMENT_SHADER, textFragmentShaderSource);
	
	textProgram = glCreateProgram();
	glAttachShader(textProgram, textVertexShader);
	glAttachShader(textProgram, textFragmentShader);
	glLinkProgram(textProgram);
	
	glDeleteShader(textVertexShader);
	glDeleteShader(textFragmentShader);	
	
	//=====
}

GLuint Shaders::getProgram() const
{
	return program;
}

GLuint Shaders::getTextProgram() const
{
	return textProgram;
}

// Private Functions

GLuint Shaders::createVertexShader() 
{
	return compileShader(GL_VERTEX_SHADER, R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
	)");
}

GLuint Shaders::createFragmentShader() {
	return compileShader(GL_FRAGMENT_SHADER, R"(
		#version 330 core
		out vec4 FragColor;
		void main() { FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }
	)");
}

void Shaders::createProgram(GLuint vertexShader, GLuint fragmentShader) {
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
}

GLuint Shaders::compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}
