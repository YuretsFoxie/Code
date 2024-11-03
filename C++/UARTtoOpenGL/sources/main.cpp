#include <windows.h>
#include <GL/glew.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <iostream>
#include <bitset>

// SerialHandler class handles serial communication
class SerialHandler {
public:
    SerialHandler() : hSerial(INVALID_HANDLE_VALUE) {}

    bool SetupSerialPort(const char* portName) {
        hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening serial port" << std::endl;
            return false;
        }

        // Setup DCB structure
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error getting serial port state" << std::endl;
            CloseHandle(hSerial);
            return false;
        }

        dcbSerialParams.BaudRate = CBR_57600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "Error setting serial port state" << std::endl;
            CloseHandle(hSerial);
            return false;
        }

        // Setup timeouts
        timeouts.ReadIntervalTimeout = 50;
        if (!SetCommTimeouts(hSerial, &timeouts)) {
            std::cerr << "Error setting timeouts" << std::endl;
            CloseHandle(hSerial);
            return false;
        }

        char startCmd = '1';
        DWORD bytesWritten;
        if (!WriteFile(hSerial, &startCmd, 1, &bytesWritten, nullptr)) {
            std::cerr << "Error writing to serial port" << std::endl;
            CloseHandle(hSerial);
            return false;
        }
        return true;
    }

    void CloseSerialPort() {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
            hSerial = INVALID_HANDLE_VALUE; // Prevent use after free
        }
    }

    HANDLE getHandle() const { return hSerial; }

private:
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
};

// ShaderManager class handles OpenGL shader compilation and management
class ShaderManager {
public:
    ShaderManager() : shaderProgram(0) {}

    bool InitializeShaders() {
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, R"(
            #version 330 core
            layout (location = 0) in vec2 aPos;
            void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
        )");

        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, R"(
            #version 330 core
            out vec4 FragColor;
            void main() { FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }
        )");

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Check for linking errors
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> log(logLength);
            glGetProgramInfoLog(shaderProgram, logLength, nullptr, log.data());
            std::cerr << "Program linking failed: " << log.data() << std::endl;
            return false;
        }

        return true;
    }

    GLuint getProgram() const { return shaderProgram; }

    ~ShaderManager() {
        if (shaderProgram) {
            glDeleteProgram(shaderProgram);
        }
    }

private:
    GLuint shaderProgram;

    GLuint CompileShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        // Error checking for shader compilation
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<char> log(logLength);
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            std::cerr << "Shader compilation failed: " << log.data() << std::endl;
        }

        return shader;
    }
};

// OpenGLBuffer class manages OpenGL vertex buffers and arrays
class OpenGLBuffer {
public:
    OpenGLBuffer() : VBO(0), VAO(0) {}

    bool PrepareBuffers(int maxPoints) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return true; // Return success status
    }

    void DrawVertices(const std::vector<float>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));
    }

    ~OpenGLBuffer() {
        if (VBO) {
            glDeleteBuffers(1, &VBO);
        }
        if (VAO) {
            glDeleteVertexArrays(1, &VAO);
        }
    }

private:
    GLuint VBO, VAO;
};

// OpenGLGraphics class initializes OpenGL and manages rendering
class OpenGLGraphics {
public:
    OpenGLGraphics() {}

    bool Initialize(HWND hwnd) {
        if (!SetupPixelFormat(hwnd)) {
            std::cerr << "Failed to setup pixel format" << std::endl;
            return false;
        }
        return shaderManager.InitializeShaders();
    }

    void PrepareBuffers(int maxPoints) {
        buffer.PrepareBuffers(maxPoints);
    }

    void DrawVertices(const std::vector<float>& vertices) {
        glUseProgram(shaderManager.getProgram());
        buffer.DrawVertices(vertices);
    }

private:
    ShaderManager shaderManager;
    OpenGLBuffer buffer;

    bool SetupPixelFormat(HWND hwnd) {
        HDC hdc = GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR), 1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
        };
        int pf = ChoosePixelFormat(hdc, &pfd);
        if (pf == 0 || !SetPixelFormat(hdc, pf, &pfd)) {
            std::cerr << "Failed to set pixel format" << std::endl;
            ReleaseDC(hwnd, hdc);
            return false;
        }

        HGLRC hglrc = wglCreateContext(hdc);
        if (!hglrc || !wglMakeCurrent(hdc, hglrc)) {
            std::cerr << "Failed to create or activate OpenGL context" << std::endl;
            ReleaseDC(hwnd, hdc);
            return false;
        }

        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            ReleaseDC(hwnd, hdc);
            return false;
        }
        
        return true; // Return success status
    }
};

// DataBuffer class manages the data received from the serial port
class DataBuffer {
public:
    DataBuffer() {
        dataBuffer.reserve(MAX_POINTS); // Reserve memory upfront
    }

    void AddData(float newData) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        dataBuffer.push_back(newData);
        if (dataBuffer.size() > MAX_POINTS) {
            dataBuffer.erase(dataBuffer.begin());
        }
    }

    void PrepareVertices(std::vector<float>& vertices) {
        std::lock_guard<std::mutex> lock(bufferMutex);
        vertices.reserve(dataBuffer.size() * 2); // Optimize memory allocation
        for (size_t i = 0; i < dataBuffer.size(); ++i) {
            float x = (2.0f * i / MAX_POINTS) - 1.0f;
            float y = convertTwosComplementToInt(std::bitset<8>(dataBuffer[i])) / 128.0f;
            vertices.push_back(x);
            vertices.push_back(y);
        }
    }

private:
    static const int MAX_POINTS = 256;
    std::vector<float> dataBuffer;
    std::mutex bufferMutex;

    int convertTwosComplementToInt(const std::bitset<8>& byte) {
        return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
    }
};

// OpenGLApplication class manages the main application flow
class OpenGLApplication {
public:
    OpenGLApplication(HINSTANCE hInstance, int nCmdShow)
        : running(true), hwnd(nullptr) {
        hwnd = SetupWindow(hInstance, nCmdShow);
        if (!glGraphics.Initialize(hwnd)) {
            std::cerr << "OpenGL Graphics initialization failed" << std::endl;
            return; // Handle failure gracefully
        }
        glGraphics.PrepareBuffers(MAX_POINTS);
    }

    void Run() {
        if (!serialHandler.SetupSerialPort("COM3")) return;
        std::thread uartThread(&OpenGLApplication::UARTThread, this);
        RenderLoop();
        running = false;
        uartThread.join();
        serialHandler.CloseSerialPort();
    }

    ~OpenGLApplication() {
        // Cleanup is now handled in destructors
    }

private:
    static const int MAX_POINTS = 256;
    static const int BATCH_SIZE = 100;

    std::atomic<bool> running;
    HWND hwnd;
    SerialHandler serialHandler;
    OpenGLGraphics glGraphics;
    DataBuffer dataBuffer;

    HWND SetupWindow(HINSTANCE hInstance, int nCmdShow) {
        WNDCLASSEX wc = {
            sizeof(WNDCLASSEX), CS_OWNDC, WndProc, 0, 0,
            GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
            "OpenGL", nullptr
        };
        RegisterClassEx(&wc);

        HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "OpenGL Application", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            nullptr, nullptr, wc.hInstance, nullptr);
        ShowWindow(hwnd, nCmdShow);
        return hwnd;
    }

    void UARTThread() {
        char buffer[1];
        DWORD bytesRead;
        while (running) {
            if (ReadFile(serialHandler.getHandle(), buffer, 1, &bytesRead, nullptr) && bytesRead > 0) {
                dataBuffer.AddData(static_cast<float>(buffer[0]));
            }
        }
    }

    void Render(HDC hdc, int& updateCounter) {
        if (++updateCounter >= BATCH_SIZE) {
            glClear(GL_COLOR_BUFFER_BIT);

            std::vector<float> vertices;
            dataBuffer.PrepareVertices(vertices);

            if (!vertices.empty()) {
                glGraphics.DrawVertices(vertices);
            }

            SwapBuffers(hdc);
            updateCounter = 0;
        }
    }

    void RenderLoop() {
        HDC hdc = GetDC(hwnd);
        int updateCounter = 0;

        while (running) {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) running = false;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            Render(hdc, updateCounter);
        }

        ReleaseDC(hwnd, hdc); // Release the device context when done
    }

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_CLOSE) {
            PostQuitMessage(0);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
};

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    OpenGLApplication app(hInstance, nCmdShow);
    app.Run();
    return 0;
}
