#include "VertexBuffer.h"
#include <GL/glew.h>
#include <cmath>

VertexBuffer::VertexBuffer(const float* verticies, int verticiesCount, const int attributes[], int attributesCount) {
	init(attributes, attributesCount);

	bind();
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(float), verticies, GL_STATIC_DRAW);
	// unbind();
}

VertexBuffer::VertexBuffer(const int attributes[], int attributesCount) {
	init(attributes, attributesCount);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_vertexBufferId);
}

void VertexBuffer::bind() {
	// std::cout << "Binding vbo: " << m_vertexBufferId << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
}

void VertexBuffer::unbind() {
	// std::cout << "Unbinding vbo: " << m_vertexBufferId << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const float* verticies, int verticiesCount) {
	// bind();
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(float), verticies, GL_STREAM_DRAW);
}

void VertexBuffer::setDataSize(int verticiesCount) {
	// bind();
	glBufferData(GL_ARRAY_BUFFER, verticiesCount * m_attributesStride * sizeof(float), NULL, GL_STREAM_DRAW);
}

void VertexBuffer::setSubData(const float* verticies, int verticiesCount, int vertexOffset) {
	// bind();
	glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * m_attributesStride * sizeof(float), verticiesCount * m_attributesStride * sizeof(float), verticies);
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
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, m_attributesStride * sizeof(float), (void*)total);

		total += attributes[i] * sizeof(float);
	}

	// unbind();
}