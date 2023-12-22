#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <GL/gl3w.h>
#include "console.h"

class Program
{
public:
    static Program& shared()
    {
        static Program p;
        return p;
    }

    void setDefault();

private:
    Program()
    {
        program = glCreateProgram();
    }

    ~Program()
    {
        glDeleteProgram(program);
    }

    GLuint loadShader(const std::string& fileName, GLenum type);
    void compile(const std::vector<GLuint>& shaders);

    void checkShader(const GLuint shader);
    void checkProgram();

    GLuint program;
};

#endif // PROGRAM_H_INCLUDED
