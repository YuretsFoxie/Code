#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <vector>
#include <atomic>

class OpenGLException: public std::runtime_error 
{
public:
    OpenGLException(const std::string& message): std::runtime_error(message) {}
};

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    int Width;          // Width of the glyph
    int Height;         // Height of the glyph
    int BearingX;       // Offset from baseline to left/top of glyph
    int BearingY;       // Offset from baseline to left/top of glyph
    long Advance;       // Offset to advance to next glyph
};

class OpenGLContext 
{
public:
    void initialize(HWND hwnd) 
    {
        if (!setupPixelFormat(hwnd))
            throw OpenGLException("failed to setup pixel format");

        if (glewInit() != GLEW_OK)
            throw OpenGLException("failed to initialize GLEW");

        if (FT_Init_FreeType(&ft))
            throw OpenGLException("could not init freetype library");

        if (FT_New_Face(ft, "fonts/ARIAL.ttf", 0, &face))
            throw OpenGLException("could not open font");

        FT_Set_Pixel_Sizes(face, 0, 48);

        // Compile and link shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        textShader = glCreateProgram();
        glAttachShader(textShader, vertexShader);
        glAttachShader(textShader, fragmentShader);
        glLinkProgram(textShader);
        checkCompileErrors(textShader, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Set up OpenGL state for text rendering
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Set up blending options
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        loadCharacters();
    }

    void renderText(const std::string& text, float x, float y, float scale, float color[3])
    {
        glUseProgram(textShader);

        // Set orthographic projection matrix
        float ortho[16] = {
            2.0f / 800, 0, 0, 0,
            0, 2.0f / 600, 0, 0,
            0, 0, -1.0f, 0,
            -1.0f, -1.0f, 0, 1.0f
        };
        glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, ortho);

        glUniform3f(glGetUniformLocation(textShader, "textColor"), color[0], color[1], color[2]);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        for (const char& c : text)
        {
            Character ch = Characters[c];

            float xpos = x + ch.BearingX * scale;
            float ypos = y - (ch.Height - ch.BearingY) * scale;

            float w = ch.Width * scale;
            float h = ch.Height * scale;
            float vertices[6][4] = {
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

            x += (ch.Advance >> 6) * scale;  // Advance cursor to next glyph
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
	void loadCharacters()
	{
		// Load the first 128 ASCII characters
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character: " << static_cast<int>(c) << std::endl;
				continue;
			}

			// Set pixel storage mode to avoid alignment issues
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Store character in map
			Character character = {
				texture,
				static_cast<int>(face->glyph->bitmap.width),  // Explicit cast to int
				static_cast<int>(face->glyph->bitmap.rows),   // Explicit cast to int
				face->glyph->bitmap_left,
				face->glyph->bitmap_top,
				face->glyph->advance.x
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    bool setupPixelFormat(HWND hwnd) 
    {
        HDC hdc = ::GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR), 1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
            32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
        };
        
        int pf = ::ChoosePixelFormat(hdc, &pfd);
        if (pf == 0 || !SetPixelFormat(hdc, pf, &pfd)) 
        {
            ::ReleaseDC(hwnd, hdc);
            throw OpenGLException("failed to set pixel format");
        }
        
        HGLRC hglrc = wglCreateContext(hdc);
        if (!hglrc || !wglMakeCurrent(hdc, hglrc)) 
        {
            ::ReleaseDC(hwnd, hdc);
            throw OpenGLException("failed to create or activate OpenGL context");
        }
        
        return true;
    }

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    FT_Library ft;
    FT_Face face;
    GLuint textShader;
    GLuint VAO, VBO;
    std::map<char, Character> Characters;

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
};

class Window
{
public:
    Window(HINSTANCE hInstance, int nCmdShow)
    {
        registerWindowClass(hInstance);
        hwnd = createWindowInstance(hInstance, nCmdShow);
    }

    void processMessages(std::atomic<bool>& isRunning)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                isRunning = false;

            if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                isRunning = false;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    HWND getHwnd() const
    {
        return hwnd;
    }

private:
    void registerWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_OWNDC, wndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "OpenGL", NULL};
        ::RegisterClassEx(&wc);
    }

    HWND createWindowInstance(HINSTANCE hInstance, int nCmdShow)
    {
        HWND hwnd = CreateWindowEx(0, "OpenGL", "OpenGL Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
        ::ShowWindow(hwnd, nCmdShow);
        return hwnd;
    }

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_CLOSE)
            PostQuitMessage(0);
        
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    HWND hwnd;
};

class Application
{
public:
    Application(HINSTANCE hInstance, int nCmdShow)
        : isRunning(true), hwnd(NULL), window(hInstance, nCmdShow), context()
    {
        hwnd = window.getHwnd();
        try
        {
            context.initialize(hwnd);
        }
        catch (const OpenGLException& e)
        {
            std::cerr << "OpenGL initialization failed: " << e.what() << std::endl;
            std::cin.get();
            return;
        }
    }

    void run()
    {
        HDC hdc = ::GetDC(hwnd);
        while (isRunning)
        {
            window.processMessages(isRunning);
            renderScene(hdc);
        }
        ::ReleaseDC(hwnd, hdc);
    }

private:
    void renderScene(HDC hdc)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float color[3] = {0.0f, 1.0f, 0.0f};
        context.renderText("Hello, OpenGL!", 25.0f, 25.0f, 1.0f, color);

        ::SwapBuffers(hdc);
    }

    std::atomic<bool> isRunning;
    HWND hwnd;
    Window window;
    OpenGLContext context;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        Application app(hInstance, nCmdShow);
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Application initialization failed: " << e.what() << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
