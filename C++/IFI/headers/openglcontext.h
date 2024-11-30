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
	
	FT_Library ft;
	FT_Face face;
	GLuint textVAO, textVBO;
	std::map<char, Character> Characters;
};

#endif // OPENGLCONTEXT_H_INCLUDED
