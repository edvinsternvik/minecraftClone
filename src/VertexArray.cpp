#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_VertexArrayId);
	bind();
}

VertexArray::~VertexArray() {

}

void VertexArray::bind() {
	glBindVertexArray(m_VertexArrayId);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}