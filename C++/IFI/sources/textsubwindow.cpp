#include <string>
#include <map>
#include "textsubwindow.h"

// Public Functions

TextSubwindow::TextSubwindow(const Origin& origin, const Size& size, const std::string& text, float scale, float color[3]):
	Subwindow(origin, size), 
	text(text), 
	scale(scale)
{
	std::copy(color, color + 3, this->color);
	setupTextRendering();
}

void TextSubwindow::render()
{
	glUseProgram(shaderProgram);
	setOrthographicProjection();
	setTextColor(color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	
	for (const char& c: text)
	{
		Character ch = Characters[c];
		renderCharacter(ch, origin.x, origin.y, scale);
		origin.x += (ch.Advance >> 6) * scale;
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Private Functions

void TextSubwindow::setupTextRendering()
{
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextSubwindow::setOrthographicProjection()
{
	float ortho[16] = 
	{
		2.0f / 1024, 0, 0, 0,
		0, 2.0f / 768, 0, 0,
		0, 0, -1.0f, 0,
		-1.0f, -1.0f, 0, 1.0f
	};
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, ortho);
}

void TextSubwindow::setTextColor(float color[3])
{
	glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color[0], color[1], color[2]);
}

void TextSubwindow::renderCharacter(const Character& ch, float x, float y, float scale)
{
	float xpos = x + ch.BearingX * scale;
	float ypos = y - (ch.Height - ch.BearingY) * scale;
	float w = ch.Width * scale;
	float h = ch.Height * scale;
	float vertices[6][4] = 
	{
		{xpos, ypos + h, 0.0f, 0.0f},
		{xpos, ypos, 0.0f, 1.0f},
		{xpos + w, ypos, 1.0f, 1.0f},
		
		{xpos, ypos + h, 0.0f, 0.0f},
		{xpos + w, ypos, 1.0f, 1.0f},
		{xpos + w, ypos + h, 1.0f, 0.0f}
	};
	
	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
