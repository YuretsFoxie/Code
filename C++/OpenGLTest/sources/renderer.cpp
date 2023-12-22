#include "renderer.h"
#include "program.h"
#include "math.h"

// Public Methods

void Renderer::draw(float time)
{
    const GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glClearBufferfv(GL_COLOR, 0, color);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Private Methods

void Renderer::onStart()
{
    glCreateVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    Program::shared().setDefault();
}

void Renderer::onFinish()
{
    glDeleteVertexArrays(1, &vertexArrayObject);
}
