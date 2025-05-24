#include "VertexBuffer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(const float* verticies, int verticiesCount, const int attributes[], int attributesCount) {
    init(attributes, attributesCount);

    bind();
    glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(unsigned int), verticies, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const int attributes[], int attributesCount) {
    init(attributes, attributesCount);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_vertexBufferId);
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const unsigned int* verticies, int verticiesCount) {
    glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(unsigned int), verticies, GL_STREAM_DRAW);
}

void VertexBuffer::setDataSize(int verticiesCount) {
    glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(unsigned int), NULL, GL_STREAM_DRAW);
}

void VertexBuffer::setSubData(const unsigned int* verticies, int verticiesCount, int vertexOffset) {
    glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * m_attributesStride * sizeof(unsigned int), verticiesCount * m_attributesStride * sizeof(unsigned int), verticies);
}

void VertexBuffer::init(const int attributes[], int attributesCount) {
    m_attributesStride = 0;
    for(int i = 0; i < attributesCount; ++i) {
        m_attributesStride += attributes[i];
    }

    glGenBuffers(1, &m_vertexBufferId);
    bind();

    int total = 0;
    for(int i = 0; i < attributesCount; ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, m_attributesStride * sizeof(unsigned int), reinterpret_cast<void*>(total));

        total += attributes[i] * sizeof(unsigned int);
    }
}
