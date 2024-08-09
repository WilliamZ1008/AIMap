#include <GL/glew.h>

#include "IndexBuffer.h"

#define ASSERT(x) if (!(x)) __debugbreak();

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_Renderer_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_Renderer_ID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
