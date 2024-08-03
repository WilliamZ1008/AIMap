#include <GL/glew.h>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    // Static draw is used for data that changes infrequently

    glGenBuffers(1, &m_Renderer_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(GLsizeiptr size){
    // Dynamic draw is used for data that changes frequently

    glGenBuffers(1, &m_Renderer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &m_Renderer_ID);
}

void VertexBuffer::Bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
}

void VertexBuffer::Unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::DynamicBufferSubData(const void* data, unsigned int size){
	glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
