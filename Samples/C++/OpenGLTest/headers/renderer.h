#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <GL/gl3w.h>

class Renderer
{
public:
    static Renderer& shared()
    {
        static Renderer r;
        return r;
    }

    void setWindowSize(int newWidth, int newHeight)
    {
        width = newWidth;
        height = newHeight;
    }

    void draw(float time);

private:
    Renderer()
    {
        gl3wInit();
        onStart();
    }

    ~Renderer()
    {
        onFinish();
    }

    void onStart();
    void onFinish();

    GLuint vertexArrayObject;

    int width;
    int height;
};

#endif // RENDERER_H_INCLUDED
