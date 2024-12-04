#ifndef OPENGLCONTEXT_H_INCLUDED
#define OPENGLCONTEXT_H_INCLUDED

#include <windows.h>
#include <map>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	GLuint TextureID;
	int Width;
	int Height;
	int BearingX;
	int BearingY;
	long Advance;
};



class OpenGLContext
{
public:
	void initialize(HWND hwnd);
	void renderText(const std::string& text, float x, float y);
	
private:
	void initializeGLEW();
	void initializeFreeType();
	void setupTextRendering();
	void loadCharacters();
	void loadCharacter(unsigned char c);
	GLuint createTexture(const FT_Bitmap& bitmap);
	void setTextureParameters();
	Character createCharacter(GLuint texture, FT_GlyphSlot glyph);
	void renderCharacter(const Character& ch, float x, float y, float scale);
	void setupPixelFormat(HWND hwnd);
	
	FT_Library ft;
	FT_Face face;
	GLuint textVAO, textVBO;
	std::map<char, Character> Characters;
};

#endif // OPENGLCONTEXT_H_INCLUDED
