#include "openglcontext.h"

// Public Functins

void OpenGLContext::initialize(HWND hwnd) 
{
	setupPixelFormat(hwnd);
	initializeGLEW();
	initializeFreeType();
	setupShaders();
	setupTextRendering();
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadCharacters();
	((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1); // enable vsync
}

void OpenGLContext::renderText(const std::string& text, float x, float y, float scale, float color[3])
{
	glUseProgram(textShader);
	setOrthographicProjection();
	setTextColor(color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	
	for (const char& c: text) 
	{
		Character ch = Characters[c];
		renderCharacter(ch, x, y, scale);
		x += (ch.Advance >> 6) * scale;
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
	
// Private Functions

void OpenGLContext::initializeGLEW()
{
	glewInit();
}

void OpenGLContext::initializeFreeType()
{
	FT_Init_FreeType(&ft);
	FT_New_Face(ft, "fonts/ARIAL.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, 24);
}

void OpenGLContext::setupShaders()
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	textShader = createShaderProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void OpenGLContext::setupTextRendering()
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

void OpenGLContext::loadCharacters()
{
	for (unsigned char c = 0; c < 128; c++)
		loadCharacter(c);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLContext::loadCharacter(unsigned char c) 
{
	FT_Load_Char(face, c, FT_LOAD_RENDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texture = createTexture(face->glyph->bitmap);
	Character character = createCharacter(texture, face->glyph);
	Characters.insert(std::pair<char, Character>(c, character));
}

GLuint OpenGLContext::createTexture(const FT_Bitmap& bitmap)
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

void OpenGLContext::setTextureParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Character OpenGLContext::createCharacter(GLuint texture, FT_GlyphSlot glyph)
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

void OpenGLContext::setOrthographicProjection()
{
	float ortho[16] = 
	{
		2.0f / 1024, 0, 0, 0,
		0, 2.0f / 768, 0, 0,
		0, 0, -1.0f, 0,
		-1.0f, -1.0f, 0, 1.0f
	};
	glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, ortho);
}

void OpenGLContext::setTextColor(float color[3])
{
	glUniform3f(glGetUniformLocation(textShader, "textColor"), color[0], color[1], color[2]);
}

void OpenGLContext::renderCharacter(const Character& ch, float x, float y, float scale)
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

void OpenGLContext::setupPixelFormat(HWND hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};
	
	int pf = ::ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pf, &pfd);
	
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
}

GLuint OpenGLContext::compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	return shader;
}

GLuint OpenGLContext::createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	return program;
}
