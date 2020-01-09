#pragma once

class VertexBuffer {
public:
	VertexBuffer(const float* verticies, int verticiesCount, const int attributes[], int attributesCount);
	VertexBuffer(const int attributes[], int attributesCount);
	~VertexBuffer();

	void addVertexAttribArray(int count);

	void bind();
	void unbind();

	void setData(const float* verticies, int verticiesCount);
	void setDataSize(int verticiesCount);
	void setSubData(const float* verticies, int verticiesCount, int vertexOffset);

private:
	void init(const int attributes[], int attributesCount);

private:
	unsigned int m_vertexBufferId;
	unsigned int m_attributesStride;
};