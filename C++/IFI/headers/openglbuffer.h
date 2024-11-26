#ifndef OPENGLBUFFER_H_INCLUDED
#define OPENGLBUFFER_H_INCLUDED

#include <vector>
#include <GL/glew.h>

class OpenGLBuffer {
public:
	OpenGLBuffer(): VBO(0), VAO(0) {}
   ~OpenGLBuffer();
	
	void prepare(int maxPoints);
	void draw(const std::vector<float>& vertices);
	
private:
	void generateBuffers();
	void bindBuffers(int maxPoints);
	void configureVertexAttribPointer();
	void unbindBuffers();
	void cleanup();
	
	GLuint VBO, VAO;
};

#endif // OPENGLBUFFER_H_INCLUDED
