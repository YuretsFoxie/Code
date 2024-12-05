#ifndef TEXTSUBWINDOW_H_INCLUDED
#define TEXTSUBWINDOW_H_INCLUDED

#include <windows.h>
#include <map>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "subwindow.h"

struct Character
{
	GLuint TextureID;
	int Width;
	int Height;
	int BearingX;
	int BearingY;
	long Advance;
};



class TextSubwindow: public Subwindow
{
public:
	// void draw() override;
	
	TextSubwindow(const Origin& origin = {0}, const Size& size = {0}): Subwindow(origin, size) {}
   ~TextSubwindow() override;
   
	void initialize(HWND hwnd);
	void draw(const std::string& text, float x, float y);
	
private:
	void initializeFreeType();
	void setupTextRendering();
	void loadCharacters();
	void loadCharacter(unsigned char c);
	GLuint createTexture(const FT_Bitmap& bitmap);
	void setTextureParameters();
	Character createCharacter(GLuint texture, FT_GlyphSlot glyph);
	void renderCharacter(const Character& ch, float x, float y, float scale);
	
	FT_Library ft;
	FT_Face face;
	GLuint VAO, VBO;
	std::map<char, Character> Characters;
};

#endif // TEXTSUBWINDOW_H_INCLUDED
