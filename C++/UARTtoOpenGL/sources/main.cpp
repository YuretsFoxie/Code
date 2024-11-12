#include <windows.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

class Settings {
public:
    Settings(const std::string& filePath) {
        readConfigFile(filePath);
    }

    std::string getSerialPort() const { return serialPort; }
    int getBaudRate() const { return baudRate; }
    int getBatchSize() const { return batchSize; }
    int getScaleFactor() const { return scaleFactor; }
    int getMaxPoints() const { return maxPoints; }

private:
    void readConfigFile(const std::string& filePath) {
        std::ifstream configFile(filePath);
        std::string line;
        while (std::getline(configFile, line)) parseConfigLine(line);
    }

    void parseConfigLine(const std::string& line) {
        auto delimiterPos = line.find(": ");
        auto name = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 2);

        if (name == "serialPort") serialPort = value;
        else if (name == "baudRate") baudRate = std::stoi(value);
        else if (name == "batchSize") batchSize = std::stoi(value);
        else if (name == "scaleFactor") scaleFactor = std::stoi(value);
        else if (name == "maxPoints") maxPoints = std::stoi(value);
    }

    std::string serialPort;
    int baudRate;
    int batchSize;
    int scaleFactor;
    int maxPoints;
};

class COMPort {
public:
    COMPort(): handle(INVALID_HANDLE_VALUE) {}
    ~COMPort() { closePort(); }

    void setup(const std::string& portName, int baudRate) {
        openPort(portName);
        configurePort(baudRate);
        setPortTimeouts();
    }

    void toggleDataTransmission(bool enable) {
        char cmd = enable ? '1' : '0';
        DWORD bytesWritten;
        ::WriteFile(handle, &cmd, 1, &bytesWritten, NULL);
    }

    HANDLE getHandle() const { return handle; }

private:
    void openPort(const std::string& portName) {
        handle = ::CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    void configurePort(int baudRate) {
        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        ::GetCommState(handle, &dcbSerialParams);
        dcbSerialParams.BaudRate = baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        ::SetCommState(handle, &dcbSerialParams);
    }

    void setPortTimeouts() {
        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout = 50;
        ::SetCommTimeouts(handle, &timeouts);
    }

    void closePort() {
        if (handle != INVALID_HANDLE_VALUE) {
            ::CloseHandle(handle);
            handle = INVALID_HANDLE_VALUE;
        }
    }

    HANDLE handle;
};

class COMPortAdapter {
public:
    COMPortAdapter(): comPort() {}

    void setup(const std::string& portName, int baudRate) {
        comPort.setup(portName, baudRate);
    }

    void toggleDataTransmission(bool enable) {
        comPort.toggleDataTransmission(enable);
    }

    HANDLE getHandle() const { return comPort.getHandle(); }

private:
    COMPort comPort;
};

struct Character {
    GLuint TextureID;
    int Width;
    int Height;
    int BearingX;
    int BearingY;
    long Advance;
};

class Shaders {
public:
    Shaders(): program(0) {}
    ~Shaders() {
        if (program) glDeleteProgram(program);
    }

    void initialize() {
        GLuint vertexShader = createVertexShader();
        GLuint fragmentShader = createFragmentShader();
        createProgram(vertexShader, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint getProgram() const { return program; }

private:
    GLuint createVertexShader() {
        return compileShader(GL_VERTEX_SHADER, R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
        )");
    }

    GLuint createFragmentShader() {
        return compileShader(GL_FRAGMENT_SHADER, R"(
            #version 330 core
            out vec4 FragColor;
            void main() { FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }
        )");
    }

    void createProgram(GLuint vertexShader, GLuint fragmentShader) {
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
    }

    GLuint compileShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        return shader;
    }

    GLuint program;
};

class OpenGLBuffer {
public:
    OpenGLBuffer(): VBO(0), VAO(0) {}
    ~OpenGLBuffer() { cleanup(); }

    void prepare(int maxPoints) {
        generateBuffers();
        bindBuffers(maxPoints);
        configureVertexAttribPointer();
        unbindBuffers();
    }

    void draw(const std::vector<float>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));
    }

private:
    void generateBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    void bindBuffers(int maxPoints) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
    }

    void configureVertexAttribPointer() {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void cleanup() {
        if (VBO) glDeleteBuffers(1, &VBO);
        if (VAO) glDeleteVertexArrays(1, &VAO);
    }

    GLuint VBO, VAO;
};

class OpenGLContext {
public:
    void initialize(HWND hwnd) {
        setupPixelFormat(hwnd);
        glewInit();
        FT_Init_FreeType(&ft);
        FT_New_Face(ft, "fonts/ARIAL.ttf", 0, &face);
        FT_Set_Pixel_Sizes(face, 0, 24);

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        textShader = glCreateProgram();
        glAttachShader(textShader, vertexShader);
        glAttachShader(textShader, fragmentShader);
        glLinkProgram(textShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &textVAO);
        glGenBuffers(1, &textVBO);
        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        loadCharacters();
    }

    void renderText(const std::string& text, float x, float y, float scale, float color[3]) {
        glUseProgram(textShader);

        float ortho[16] = {
            2.0f / 800, 0, 0, 0,
            0, 2.0f / 600, 0, 0,
            0, 0, -1.0f, 0,
            -1.0f, -1.0f, 0, 1.0f
        };
        glUniformMatrix4fv(glGetUniformLocation(textShader, "projection"), 1, GL_FALSE, ortho);

        glUniform3f(glGetUniformLocation(textShader, "textColor"), color[0], color[1], color[2]);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVAO);

        for (const char& c : text) {
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
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    void loadCharacters() {
        for (unsigned char c = 0; c < 128; c++) {
            FT_Load_Char(face, c, FT_LOAD_RENDER);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

            Character character = {
                texture,
                static_cast<int>(face->glyph->bitmap.width),
                static_cast<int>(face->glyph->bitmap.rows),
                face->glyph->bitmap_left,
                face->glyph->bitmap_top,
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setupPixelFormat(HWND hwnd) {
        HDC hdc = ::GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR), 1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
            32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
        };

        int pf = ::ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pf, &pfd);
        HGLRC hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);
    }

    FT_Library ft;
    FT_Face face;
    GLuint textShader;
    GLuint textVAO, textVBO;
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

class Graphics {
public:
    void initialize(HWND hwnd, const Settings& settings) {
        context.initialize(hwnd);
        shaders.initialize();
        prepareBuffers(settings.getMaxPoints());
    }

    void prepareBuffers(int maxPoints) {
        buffer.prepare(maxPoints);
    }

    void drawVertices(const std::vector<float>& vertices) {
        glUseProgram(shaders.getProgram());
        buffer.draw(vertices);
    }

    void drawText(const std::string& text, float x, float y, float scale, float color[3]) {
        context.renderText(text, x, y, scale, color);
    }

private:
    Shaders shaders;
    OpenGLBuffer buffer;
    OpenGLContext context;
};

class DataBuffer {
public:
    DataBuffer(int maxPoints, int scaleFactor) : MAX_POINTS(maxPoints), SCALE_FACTOR(scaleFactor) {
        dataBuffer.reserve(MAX_POINTS);
    }

    void push(float newData) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataBuffer.push_back(newData);
        if (dataBuffer.size() > MAX_POINTS)
            dataBuffer.erase(dataBuffer.begin());
    }

    void prepare(std::vector<float>& vertices) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        vertices.reserve(dataBuffer.size() * 2);
        preComputeVertices(vertices);
    }

private:
    void preComputeVertices(std::vector<float>& vertices) {
        const float scaleFactor = 2.0f / MAX_POINTS;
        for (size_t i = 0; i < dataBuffer.size(); ++i) {
            float x = (scaleFactor * i) - 1.0f;
            float y = convertTwosComplementToInt(std::bitset<8>(dataBuffer[i])) / static_cast<float>(SCALE_FACTOR);
            vertices.push_back(x);
            vertices.push_back(y);
        }
    }

    int convertTwosComplementToInt(const std::bitset<8>& byte) {
        return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
    }

    const int MAX_POINTS;
    const int SCALE_FACTOR;
    std::vector<float> dataBuffer;
    std::mutex bufferMutex;
};

class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow) {
        registerWindowClass(hInstance);
        hwnd = createWindowInstance(hInstance, nCmdShow);
    }

    void processMessages(std::atomic<bool>& isRunning, COMPortAdapter& port, std::atomic<bool>& isReceiving) {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                isRunning = false;

            if (msg.message == WM_KEYDOWN) {
                if (msg.wParam == VK_ESCAPE)
                    isRunning = false;

                if (msg.wParam == VK_F1) {
                    isReceiving = !isReceiving;
                    port.toggleDataTransmission(isReceiving);
                }
            }

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    HWND getHwnd() const { return hwnd; }

private:
    void registerWindowClass(HINSTANCE hInstance) {
        WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_OWNDC, wndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "OpenGL", NULL};
        ::RegisterClassEx(&wc);
    }

    HWND createWindowInstance(HINSTANCE hInstance, int nCmdShow) {
        HWND hwnd = CreateWindowEx(0, "OpenGL", "Foxie Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
        ::ShowWindow(hwnd, nCmdShow);
        return hwnd;
    }

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_CLOSE)
            PostQuitMessage(0);

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    HWND hwnd;
};

class Renderer {
public:
    Renderer(Graphics& graphics, DataBuffer& buffer, int batchSize) : graphics(graphics), buffer(buffer), BATCH_SIZE(batchSize) {}

    void renderFrame(HDC hdc, int& updateCounter, std::atomic<bool>& isRunning) {
        updateCounter++;
        if (updateCounter >= BATCH_SIZE) {
            drawVertices();
            drawText();
            ::SwapBuffers(hdc);
            updateCounter = 0;
        }
    }

private:
    void drawVertices() {
        glClear(GL_COLOR_BUFFER_BIT);
        std::vector<float> vertices;
        buffer.prepare(vertices);

        if (!vertices.empty())
            graphics.drawVertices(vertices);
    }

    void drawText() {
        float color[3] = {0.0f, 1.0f, 0.0f};
        graphics.drawText("UART Data Visualization", 25.0f, 25.0f, 1.0f, color);
    }

    Graphics& graphics;
    DataBuffer& buffer;
    const int BATCH_SIZE;
};

class Application {
public:
    Application(HINSTANCE hInstance, int nCmdShow, const Settings& settings)
        : isRunning(true), isReceiving(false), hwnd(NULL), settings(settings), window(hInstance, nCmdShow), graphics(), buffer(settings.getMaxPoints(), settings.getScaleFactor()), renderer(graphics, buffer, settings.getBatchSize()), portAdapter() {
        hwnd = window.getHwnd();
        graphics.initialize(hwnd, settings);
        graphics.prepareBuffers(settings.getMaxPoints());
    }

    void run() {
        portAdapter.setup(settings.getSerialPort(), settings.getBaudRate());
        runCOMPortThread();
        runLoop();
    }

private:
    void runCOMPortThread() {
        std::thread portThread(&Application::runCOMPort, this);
        portThread.detach();
    }

    void runCOMPort() {
        char array[1];
        DWORD bytesRead;

        while (isRunning)
            if (isReceiving) {
                ::ReadFile(portAdapter.getHandle(), array, 1, &bytesRead, NULL);
                if (bytesRead > 0)
                    buffer.push(static_cast<float>(array[0]));
            }
    }

    void runLoop() {
        HDC hdc = ::GetDC(hwnd);
        int updateCounter = 0;
        while (isRunning) {
            window.processMessages(isRunning, portAdapter, isReceiving);
            renderer.renderFrame(hdc, updateCounter, isRunning);
        }
        ::ReleaseDC(hwnd, hdc);
    }

    const Settings& settings;
    std::atomic<bool> isRunning;
    std::atomic<bool> isReceiving;
    HWND hwnd;
    COMPortAdapter portAdapter;
    Graphics graphics;
    DataBuffer buffer;
    Window window;
    Renderer renderer;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Settings settings("settings.ini");
    Application app(hInstance, nCmdShow, settings);
    app.run();
    return EXIT_SUCCESS;
}
