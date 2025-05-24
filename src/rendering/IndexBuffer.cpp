#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const int* data, unsigned int count) {
    init();

    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(int), data, GL_STATIC_DRAW);
    unbind();
}

IndexBuffer::IndexBuffer() {
    init();
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_indexBufferId);
}

void IndexBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setData(const int* data, unsigned int count) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(int), data, GL_DYNAMIC_DRAW);
}

void IndexBuffer::init() {
    glGenBuffers(1, &m_indexBufferId);
}