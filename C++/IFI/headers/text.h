#ifndef TEXTSUBWINDOW_H_INCLUDED
#define TEXTSUBWINDOW_H_INCLUDED

#include <map>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "viewportparameters.h"

class Text
{
public:
	Text(const ViewPortParameters& parameters): parameters(parameters) {}
   ~Text();
	
	void setup();
	void draw();
	void print(const std::string& message);
	
private:
	struct Character
	{
		GLuint TextureID;
		int Width;
		int Height;
		int BearingX;
		int BearingY;
		long Advance;
	};
	
	void drawBuffer(const std::string& text, float x, float y);
	void initializeFreeType();
	void setupTextRendering();
	void loadCharacters();
	void loadCharacter(unsigned char c);
	GLuint createTexture(const FT_Bitmap& bitmap);
	void setTextureParameters();
	Character createCharacter(GLuint texture, FT_GlyphSlot glyph);
	void renderCharacter(const Character& ch, float x, float y);
	
	const float leftEdge = 16;
	const float rowHeight = 24;
	const float fontSize = 20;
	
	ViewPortParameters parameters;
	FT_Library ft;
	FT_Face face;
	GLuint VAO, VBO;
	std::map<char, Character> characters;
	std::vector<std::string> buffer;
	int bufferSize = 0;
};

#endif // TEXTSUBWINDOW_H_INCLUDED
