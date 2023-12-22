///////////////////////////////////////////////////////////////////////////////
// ControllerGL.cpp
// ================
// Derived Controller class for OpenGL window
// It is the controller of OpenGL rendering window. It initializes DC and RC,
// when WM_CREATE called, then, start new thread for OpenGL rendering loop.
//
// When this class is constructed, it gets the pointers to model and view
// components.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gamil.com)
// CREATED: 2013-01-14
// UPDATED: 2021-10-03
///////////////////////////////////////////////////////////////////////////////

#include <process.h>                                // for _beginthreadex()
#include <string>
#include <sstream>
#include "ControllerGL.h"
#include "wcharUtil.h"
#include "Log.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
ControllerGL::ControllerGL(ModelGL* model, ViewGL* view) : model(model), view(view),
                                                           loopFlag(false), mouseHovered(false)
{
    // get app(exe) dir
    int bufferSize = 1024;
    wchar_t* buffer = new wchar_t[bufferSize];  // tmp buffer
    ::GetModuleFileName(::GetModuleHandle(0), buffer, bufferSize);
    std::wstring filePath = buffer;
    std::size_t index = filePath.find_last_of(L"/\\");
    if(index != std::string::npos)
        appPath = filePath.substr(0, index+1);
    delete [] buffer;   // dealloc tmp buffer
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_DESTROY
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::destroy()
{
    // wait for rendering thread is terminated
    loopFlag = false;
    glThread.join();

    Win::log("OpenGL window is destroyed.");
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_CREATE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::create()
{
    // create a OpenGL rendering context
    if(!view->createContext(handle, 32, 24, 8, 8))
    {
        Win::log(L"[ERROR] Failed to create OpenGL rendering context from ControllerGL::create().");
        return -1;
    }

    // create a thread for OpenGL rendering
    glThread = std::thread(&ControllerGL::runThread, this);
    loopFlag = true;
    Win::log(L"Created a rendering thread for OpenGL.");

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_PAINT
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::paint()
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_COMMAND
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::command(int id, int cmd, LPARAM msg)
{
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse down
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::lButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    if(state == MK_LBUTTON)
    {
        model->setMouseLeft(true);
    }

    // set focus to receive wm_mousewheel event
    //::SetFocus(handle);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle Left mouse up
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::lButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    model->setMouseLeft(false);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse down
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::rButtonDown(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    if(state == MK_RBUTTON)
    {
        model->setMouseRight(true);
    }

    // set focus to receive wm_mousewheel event
    //::SetFocus(handle);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle reft mouse up
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::rButtonUp(WPARAM state, int x, int y)
{
    // update mouse position
    model->setMousePosition(x, y);

    model->setMouseRight(false);

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::mouseMove(WPARAM state, int x, int y)
{
    // for tracking mouse hover/leave tracking
    if(!mouseHovered)
    {
        // mouse hover/leave tracking
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = handle;
        tme.dwHoverTime = HOVER_DEFAULT;
        ::TrackMouseEvent(&tme);
        mouseHovered = true;
    }

    if(state == MK_LBUTTON)
    {
        model->rotateCamera(x, y);
    }
    if(state == MK_RBUTTON)
    {
        model->shiftCamera(x, y);
    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEHOVER
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::mouseHover(int state, int x, int y)
{
    mouseHovered = true;

    // set focus to receive wm_mousewheel event
    //::SetFocus(handle);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSELEAVE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::mouseLeave()
{
    mouseHovered = false;
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
// mouse scrolling up is positive delta
// trackpad scrolling up is negative delta
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::mouseWheel(int state, int delta, int x, int y)
{
    if(mouseHovered)
    {
        // reverse delta, so move away while scrolling up
        model->zoomCameraDelta(-delta / 120.0f);
        //Win::log(L"delta: %d", delta);
    }
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_SIZE
///////////////////////////////////////////////////////////////////////////////
LRESULT ControllerGL::size(int w, int h, WPARAM wParam)
{
    model->setWindowSize(w, h);
    Win::log(L"Changed OpenGL rendering window size: %dx%d.", w, h);
    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// opengl rendering thread
// initialize OpenGL states and start rendering loop
///////////////////////////////////////////////////////////////////////////////
void ControllerGL::runThread()
{
    // init OpenGL stuff ==================================
    // set the current RC in this thread
    ::wglMakeCurrent(view->getDC(), view->getRC());

    // initialize OpenGL states
    model->init();
    Win::log(L"Initialized OpenGL states.");

    // check supported OpenGL extensions
    if(model->isAnisotropicSupported())
    {
        Win::log("Anisotropic filter enabled: %.1f", model->getAnisotropy());
    }
    else
    {
        Win::log(L"[WARNING] GL_ARB_texture_filter_anisotropic is not supported.");
    }

    if(model->isShaderSupported())
    {
        if(model->isShaderReady())
            Win::log("Use GL_ARB_shader_objects extension.");
        else
            Win::log("GL_ARB_shader_objects is supported, but failed to compile shaders.");
    }
    else
    {
        Win::log(L"[ERROR] Failed to initialize GLSL.");
    }

    if(model->isVboSupported())
        Win::log("Use GL_ARB_vertex_buffer_object extension.");
    else
        Win::log(L"[ERROR] Failed to initialize VBO.");

    if(model->isFboSupported())
        Win::log("Use GL_ARB_framebuffer_object extension.");
    else
        Win::log(L"[ERROR] Failed to initialize FBO.");
    Win::log(model->getFboStatus().c_str());


    // load default texture
    //wchar_t cwd[512];
    //::GetCurrentDirectory(512, cwd);
    std::string texture = toChar(appPath.c_str());
    texture += "/data/grid512.png";
    Win::log("Loading default texture: %s", texture.c_str());
    model->loadDefaultTexture(texture);

    // rendering loop
    Win::log(L"Entering OpenGL rendering loop...");
    while(loopFlag)
    {
        //std::this_thread::yield();      // yield to other processes or threads
        std::this_thread::sleep_for(std::chrono::milliseconds(33));  // yield to other processes or threads
        model->draw();
        view->swapBuffers();
    }

    // close OpenGL Rendering Context (RC)
    view->closeContext(handle);
    ::wglMakeCurrent(0, 0);             // unset RC
    Win::log(L"Closed OpenGL context.");

    Win::log(L"Exit OpenGL rendering thread.");
}
