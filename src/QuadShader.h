#pragma once

#include <string>

#include "glm/glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Config.h"
#include "Structs.h"
#include "Utils.h"


#include <iostream>

class QuadShader {
private:
	Type m_Type;
	VertexArray* m_VA;
	VertexBuffer* m_VB;
	VertexBufferLayout* m_Layout;
	IndexBuffer* m_IB;
	Shader* m_Shader;
	

public:
	QuadShader(const std::string& filepath, Type type);
	~QuadShader();

	void AddBuffer();

	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void Bind();
	void Draw(const void* data, unsigned int elementSize, unsigned int indexNumber);

	template<typename T>
	void PushLayout(unsigned int count);

	template<>
	void PushLayout<float>(unsigned int count);
};

