#pragma once

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void bind();
    void unbind();

private:
    unsigned int m_VertexArrayId;
};