#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
switch (uMsg) {
case WM_DESTROY:
PostQuitMessage(0);
return 0;
default:
return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
}

void EnableOpenGL(HWND hwnd, HDC* hdc, HGLRC* hrc) {
PIXELFORMATDESCRIPTOR pfd;
int iFormat;

*hdc = GetDC(hwnd);

ZeroMemory(&pfd, sizeof(pfd));
pfd.nSize = sizeof(pfd);
pfd.nVersion = 1;
pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
pfd.iPixelType = PFD_TYPE_RGBA;
pfd.cColorBits = 24;
pfd.cDepthBits = 16;
pfd.iLayerType = PFD_MAIN_PLANE;

iFormat = ChoosePixelFormat(*hdc, &pfd);
SetPixelFormat(*hdc, iFormat, &pfd);

*hrc = wglCreateContext(*hdc);
wglMakeCurrent(*hdc, *hrc);

glewInit();
}

void DisableOpenGL(HWND hwnd, HDC hdc, HGLRC hrc) {
wglMakeCurrent(NULL, NULL);
wglDeleteContext(hrc);
ReleaseDC(hwnd, hdc);
}

GLuint CompileShader(GLenum type, const char* source) {
GLuint shader = glCreateShader(type);
glShaderSource(shader, 1, &source, NULL);
glCompileShader(shader);

GLint success;
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
char infoLog[512];
glGetShaderInfoLog(shader, 512, NULL, infoLog);
std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
}
return shader;
}

GLuint CreateShaderProgram() {
GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

GLint success;
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (!success) {
char infoLog[512];
glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
}

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);

return shaderProgram;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
WNDCLASS wc;
HWND hwnd;
HDC hdc;
HGLRC hrc;
MSG msg;
BOOL quit = FALSE;

wc.style = CS_OWNDC;
wc.lpfnWndProc = WindowProc;
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc.hInstance = hInstance;
wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
wc.lpszMenuName = NULL;
wc.lpszClassName = "GLSample";
RegisterClass(&wc);

hwnd = CreateWindow(
"GLSample", "OpenGL Sample",
WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
0, 0, 800, 600,
NULL, NULL, hInstance, NULL);

EnableOpenGL(hwnd, &hdc, &hrc);

GLuint shaderProgram = CreateShaderProgram();

float vertices[] = {
-0.5f, -0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
0.0f,  0.5f, 0.0f
};

GLuint VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

while (!quit) {
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
if (msg.message == WM_QUIT) {
quit = TRUE;
} else {
TranslateMessage(&msg);
DispatchMessage(&msg);
}
} else {
glClear(GL_COLOR_BUFFER_BIT);

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);

SwapBuffers(hdc);
}
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);

DisableOpenGL(hwnd, hdc, hrc);
DestroyWindow(hwnd);

return msg.wParam;
}
