#include "shaderprogram.h"
#include <iostream>

// Public Functions

GLuint ShaderProgram::create()
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "error: program linking failed\n" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
}

// Private Functions

GLuint ShaderProgram::compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "error: shader compilation failed\n" << infoLog << std::endl;
	}
	
	return shader;
}
