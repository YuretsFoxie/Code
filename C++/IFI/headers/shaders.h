#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <GL/glew.h>

class Shaders
{
public:
	Shaders() {}
   ~Shaders();
	
	GLuint getGraphProgram() const;
	GLuint getTextProgram() const;
	
private:
	GLuint compileShader(GLenum type, const char* source);
	GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);
	
	const char* graphVertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
	)";
	
	const char* graphFragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		void main() { FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }
	)";
	
	const char* textVertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec4 vertex;
		uniform mat4 projection;
		out vec2 TexCoords;
		
		void main() {
			gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
			TexCoords = vertex.zw;
		}
	)";
	
	const char* textFragmentShaderSource = R"(
		#version 330 core
		in vec2 TexCoords;
		out vec4 color;
		
		uniform sampler2D text;
		uniform vec3 textColor;
		
		void main() {
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
			color = vec4(textColor, 1.0) * sampled;
		}
	)";
	
	GLuint graphProgram;
	GLuint textProgram;	
};

#endif // SHADERS_H_INCLUDED