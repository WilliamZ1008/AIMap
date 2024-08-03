#pragma once

class VertexBuffer {
private:
	unsigned int m_Renderer_ID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(GLsizeiptr size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void DynamicBufferSubData(const void* data, unsigned int size);
};