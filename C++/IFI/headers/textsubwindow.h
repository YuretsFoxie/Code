#ifndef TEXTSUBWINDOW_H_INCLUDED
#define TEXTSUBWINDOW_H_INCLUDED

#include <GL/glew.h>
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
	TextSubwindow(const Origin& origin, const Size& size, const std::string& text, float scale, float color[3]);
	void render() override;
	
private:
	void setupTextRendering();
	void setOrthographicProjection();
	void setTextColor(float color[3]);
	void renderCharacter(const Character& ch, float x, float y, float scale);
	
	std::string text;
	float scale;
	float color[3];
	GLuint shaderProgram;
	GLuint textVAO, textVBO;
	std::map<char, Character> Characters;
};

#endif // TEXTSUBWINDOW_H_INCLUDED
