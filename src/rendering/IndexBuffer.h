#pragma once

class IndexBuffer {
public:
	IndexBuffer(const int* data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void bind();
	void unbind();

	void setData(const int* data, unsigned int count);

private:
	void init();

private:
	unsigned int m_indexBufferId;
};