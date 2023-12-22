#include "application.h"
#include "renderer.h"
#include "console.h"
#include "timer.h"

HGLRC Application::hRC;

// Public Methods

WPARAM Application::run(HINSTANCE instance)
{
    hInstance = instance;
    Console::shared();
	
    registerWindowClass();
    createWindow();
    enableOpenGL();
    runMainLoop();
    disableOpenGL();
    destroyWindow();
	
    return msg.wParam;
}

// Private Methods

LRESULT CALLBACK Application::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_SIZE:
        if(hRC)
            Renderer::shared().setWindowSize(LOWORD(lParam), HIWORD(lParam));

        break;

    case WM_CLOSE:
        ::PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_ESCAPE:
            ::PostQuitMessage(0);
            return 0;

        default: Console::shared().read(wParam);
        }
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void Application::registerWindowClass()
{
    WNDCLASS wc;

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ClassName";

    ::RegisterClass(&wc);
}

void Application::createWindow()
{
    hWnd = ::CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                            TEXT("ClassName"),
                            TEXT("Goblin Window"),
                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                            0, 0,
                            800, 600,
                            NULL,
                            NULL,
                            hInstance,
                            NULL);
}

void Application::enableOpenGL()
{
    PIXELFORMATDESCRIPTOR pfd;
    hDC = ::GetDC(hWnd);

    ::ZeroMemory(&pfd, sizeof (pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int iFormat = ::ChoosePixelFormat(hDC, &pfd);
    ::SetPixelFormat(hDC, iFormat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
}

void Application::runMainLoop()
{
    Timer timer = Timer();

    while(true)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        Renderer::shared().draw(timer.getTime());
        ::SwapBuffers(hDC);
    }
}

void Application::disableOpenGL()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ::ReleaseDC(hWnd, hDC);
}

void Application::destroyWindow()
{
    ::DestroyWindow(hWnd);
}
