#ifndef OPENGLCONTEXT_H_INCLUDED
#define OPENGLCONTEXT_H_INCLUDED

#include <windows.h>
#include <map>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "textsubwindow.h" // temp, for the Character struct

class OpenGLContext
{
public:
	void initialize(HWND hwnd);
	void renderText(const std::string& text, float x, float y, float scale, float color[3]);
	
private:
	void initializeGLEW();
	void initializeFreeType();
	void setupShaders();
	void setupTextRendering();
	void loadCharacters();
	void loadCharacter(unsigned char c);
	GLuint createTexture(const FT_Bitmap& bitmap);
	void setTextureParameters();
	Character createCharacter(GLuint texture, FT_GlyphSlot glyph);
	void setOrthographicProjection();
	void setTextColor(float color[3]);
	void renderCharacter(const Character& ch, float x, float y, float scale);
	void setupPixelFormat(HWND hwnd);
	GLuint compileShader(GLenum type, const char* source);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec4 vertex;
		uniform mat4 projection;
		out vec2 TexCoords;
		
		void main() {
			gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
			TexCoords = vertex.zw;
		}
	)";
	
	const char* fragmentShaderSource = R"(
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
	
	FT_Library ft;
	FT_Face face;
	GLuint textShader;
	GLuint textVAO, textVBO;
	std::map<char, Character> Characters;
};

#endif // OPENGLCONTEXT_H_INCLUDED
