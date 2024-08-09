#include "QuadShader.h"

QuadShader::QuadShader(const std::string& filepath, Type type)
    :m_Type(type), m_VA(), m_VB(), m_Layout(), m_IB(), m_Shader() {
    unsigned int* indeces;

    switch (type) {
    case Type::Vertex:
        m_VA = new VertexArray();
        m_VB = new VertexBuffer(MAX_VERTEX_COUNT * sizeof(Vertex));
        m_Layout = new VertexBufferLayout();
        indeces = new unsigned int[MAX_VERTEX_INDEX_COUNT];
        InitQuadIndeces(indeces, MAX_VERTEX_INDEX_COUNT);
        m_IB = new IndexBuffer(indeces, MAX_VERTEX_INDEX_COUNT);
        m_Shader = new Shader(filepath);
        break;
    case Type::Edge:
        m_VA = new VertexArray();
        m_VB = new VertexBuffer(MAX_EDGE_COUNT * sizeof(Edge));
        m_Layout = new VertexBufferLayout();
        indeces = new unsigned int[MAX_EDGE_INDEX_COUNT];
        InitQuadIndeces(indeces, MAX_EDGE_INDEX_COUNT);
        m_IB = new IndexBuffer(indeces, MAX_EDGE_INDEX_COUNT);
        m_Shader = new Shader(filepath);
        break;
    default:
        break;
    }

}

QuadShader::~QuadShader(){
}

void QuadShader::AddBuffer(){
    m_VA->AddBuffer(*m_VB, *m_Layout);

}

void QuadShader::SetUniform1f(const std::string& name, float value){
    m_Shader->SetUniform1f(name, value);
}

void QuadShader::SetUniform3f(const std::string& name, float v0, float v1, float v2){
    m_Shader->SetUniform3f(name, v0, v1, v2);
}

void QuadShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix){
    m_Shader->SetUniformMat4f(name, matrix);
}

void QuadShader::Bind(){
    m_Shader->Bind();
	m_VA->Bind();
	m_IB->Bind();
}

void QuadShader::Draw(const void* data, unsigned int elementSize, unsigned int indexNumber) {
    m_VB->DynamicBufferSubData(data, elementSize);
    glDrawElements(GL_TRIANGLES, indexNumber, GL_UNSIGNED_INT, nullptr);
}

template<typename T>
void QuadShader::PushLayout(unsigned int count) {
    static_assert(false);
}

template<>
void QuadShader::PushLayout<float>(unsigned int count) {
    m_Layout->Push<float>(count);
}

template<>
void QuadShader::PushLayout<char>(unsigned int count) {
    m_Layout->Push<char>(count);
}