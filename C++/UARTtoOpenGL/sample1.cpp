#include <windows.h>
#include <GL/glew.h>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <atomic>
#include <iostream>
#include <bitset>

int convertTwosComplementToInt(const std::bitset<8> byte)
{
	return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}

// UART thread data buffer and mutex
std::vector<float> dataBuffer;
std::mutex bufferMutex;
std::atomic<bool> running(true);
const int MAX_POINTS = 256;
const int BATCH_SIZE = 100;

HANDLE hSerial;
DCB dcbSerialParams = { 0 };
COMMTIMEOUTS timeouts = { 0 };

bool SetupSerialPort(const char* portName) {
    hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return false;
    }

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
	
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
	
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting serial port timeouts" << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    // Send '1' to start UART transmission
    char startCmd = '1';
    DWORD bytesWritten;
    if (!WriteFile(hSerial, &startCmd, 1, &bytesWritten, nullptr)) {
        std::cerr << "Error sending start command" << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            running = false;
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void UARTThread() {
    char buffer[1];
    DWORD bytesRead;
    while (running) {
        if (ReadFile(hSerial, buffer, 1, &bytesRead, nullptr) && bytesRead > 0) {
            float newData = static_cast<float>(buffer[0]);
            std::lock_guard<std::mutex> lock(bufferMutex);
            dataBuffer.push_back(newData);
            if (dataBuffer.size() > MAX_POINTS) {
                dataBuffer.erase(dataBuffer.begin());
            }
        }
		
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void RenderLoop(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    glewInit();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_POINTS * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main()
    {
        gl_Position = vec4(aPos, 0.0, 1.0);
    })";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    })";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    MSG msg = {0};
    std::thread uart(UARTThread);

    int updateCounter = 0;

    while (running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		
		// std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 8 is for approx 128 Hz
		
        updateCounter++;
        if (updateCounter >= BATCH_SIZE) {
            glClear(GL_COLOR_BUFFER_BIT);

            std::vector<float> vertices;
            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                size_t dataSize = dataBuffer.size();
                for (size_t i = 0; i < dataSize; ++i) {
                    float x = (2.0f * i / MAX_POINTS) - 1.0f; // Adjust to keep within -1 to 1 range
					float y = convertTwosComplementToInt(std::bitset<8>(dataBuffer[i])) / 128.0f;
					
                    vertices.push_back(x);
                    vertices.push_back(y);
                }
            }

            if (!vertices.empty()) {
                std::cout << "Drawing " << vertices.size() / 2 << " points." << std::endl;

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

                glUseProgram(shaderProgram);
                glBindVertexArray(VAO);
                glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));
            } else {
                std::cout << "No vertices to draw." << std::endl;
            }

            SwapBuffers(hdc);

            updateCounter = 0;
        }
    }

    running = false;
    uart.join();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    CloseHandle(hSerial);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_OWNDC, WndProc, 0, 0, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "OpenGL", nullptr };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "OpenGL Triangle", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);
    ShowWindow(hwnd, nCmdShow);

    if (!SetupSerialPort("COM3")) {
        return 1; // Exit if serial port setup fails
    }

    RenderLoop(hwnd);

    return 0;
}
