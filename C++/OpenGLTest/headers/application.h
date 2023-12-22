#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <windows.h>

class Application
{
public:
    static Application& shared()
    {
        static Application app;
        return app;
    }

    WPARAM run(HINSTANCE instance);

private:
    Application() {}

    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void registerWindowClass();
    void createWindow();
    void enableOpenGL();
    void runMainLoop();
    void disableOpenGL();
    void destroyWindow();

    static HGLRC hRC;
    HINSTANCE hInstance;
    HWND hWnd;
    HDC hDC;
    MSG msg;
};

#endif // APPLICATION_H_INCLUDED
