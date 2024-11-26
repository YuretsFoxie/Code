#include "graphsubwindow.h"

// Public Functions

void GraphSubwindow::render()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(data.size() / 2));
}
