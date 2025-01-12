#include "text.h"

// Public Functions

Text::Text(const ViewPortParameters& parameters): parameters(parameters)
{
	initializeFreeType();
	setupTextRendering();
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadCharacters();
	bufferSize = parameters.height / rowHeight;
}
	
Text::~Text()
{
	if (VAO) 
	{	
		glDeleteVertexArrays(1, &VAO);
	}
	if (VBO) 
	{
		glDeleteBuffers(1, &VBO);
	}
}

void Text::draw()
{
	glViewport(parameters.x, parameters.y, parameters.width, parameters.height);
	
	int size = buffer.size();
	float height = parameters.height;
	
	for (int i = 0; i < size; i++)
		drawBuffer(buffer[i], leftEdge, height - (i + 1) * rowHeight);
}

void Text::print(const std::string& message)
{
	buffer.push_back(message);
	
	if (buffer.size() == bufferSize)
		buffer.erase(buffer.begin());
}

ViewPortParameters Text::getParameters()
{
	return parameters;
}

// Private Functions

void Text::drawBuffer(const std::string& text, float x, float y)
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	
	for (const char& c: text) 
	{
		Character ch = characters[c];
		renderCharacter(ch, x, y);
		x += (ch.Advance >> 6);
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::initializeFreeType()
{
	FT_Init_FreeType(&ft);
	FT_New_Face(ft, "fonts/ARIAL.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, fontSize);
}

void Text::setupTextRendering()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::loadCharacters()
{
	for (unsigned char c = 0; c < 128; c++)
		loadCharacter(c);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::loadCharacter(unsigned char c) 
{
	FT_Load_Char(face, c, FT_LOAD_RENDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texture = createTexture(face->glyph->bitmap);
	Character character = createCharacter(texture, face->glyph);
	characters.insert(std::pair<char, Character>(c, character));
}

GLuint Text::createTexture(const FT_Bitmap& bitmap)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		bitmap.width,
		bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		bitmap.buffer
	);
	
	setTextureParameters();
	return texture;
}

void Text::setTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Text::Character Text::createCharacter(GLuint texture, FT_GlyphSlot glyph)
{
	return {
		texture,
		static_cast<int>(glyph->bitmap.width),
		static_cast<int>(glyph->bitmap.rows),
		glyph->bitmap_left,
		glyph->bitmap_top,
		glyph->advance.x
	};
}

void Text::renderCharacter(const Character& ch, float x, float y)
{
	float xpos = x + ch.BearingX;
	float ypos = y - (ch.Height - ch.BearingY);
	float w = ch.Width;
	float h = ch.Height;
	
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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
