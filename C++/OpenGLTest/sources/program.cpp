#include "program.h"
#include "file.h"

// Public Methods

void Program::setDefault()
{
    GLuint vertexShader = loadShader("Vertex Shader", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader("Fragment Shader", GL_FRAGMENT_SHADER);

    compile({vertexShader, fragmentShader});
}

// Private Methods

GLuint Program::loadShader(const string& fileName, GLenum type)
{
    File file("shaders/" + fileName + ".txt");
    vector<string> strings = file.load();

    string code;
    for(unsigned int i = 0; i < strings.size(); i++)
        code += strings[i] + "\n";

    const char *c_str = code.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    checkShader(shader);
    return shader;
}

void Program::compile(const vector<GLuint>& shaders)
{
    GLuint program = glCreateProgram();

    for(unsigned int i = 0; i < shaders.size(); i++)
        glAttachShader(program, shaders[i]);

    glLinkProgram(program);
    //checkProgram();

    for(unsigned int i = 0; i < shaders.size(); i++)
        glDeleteShader(shaders[i]);

    glUseProgram(program);
}

void Program::checkShader(const GLuint shader)
{
    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(!status)
    {
        char buffer[4096] = {};
        glGetShaderInfoLog(shader, 4096, NULL, buffer);
        glDeleteShader(shader);

        Console::shared().print(buffer);
    }
}

void Program::checkProgram()
{
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if(!status)
    {
        char buffer[4096] = {};
        glGetProgramInfoLog(program, 4096, NULL, buffer);
        glDeleteProgram(program);

        Console::shared().print(buffer);
    }
}
