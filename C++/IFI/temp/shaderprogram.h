#ifndef GRAPH_H_SHADERPROGRAM
#define GRAPH_H_SHADERPROGRAM

#include <GL/glew.h>

class ShaderProgram
{
public:
    GLuint create();
	
private:
	GLuint compileShader(GLenum type, const char* source);
	
	const char* vertexShaderSource = 
	R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;
		void main()
		{
			gl_Position = vec4(aPos, 1.0);
		}
	)";
	
	const char* fragmentShaderSource = 
	R"(
		#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	)";
};

#endif // GRAPH_H_SHADERPROGRAM
