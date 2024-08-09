#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer {
private:
	unsigned int m_Renderer_ID;
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(GLsizeiptr size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void DynamicBufferSubData(const void* data, GLsizeiptr size);
};