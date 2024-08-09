#include "QuadRenderer.h"

QuadRenderer::QuadRenderer(const std::string& filepath, Type type)
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

        PushLayout<float>(2);
        PushLayout<float>(2);
        PushLayout<float>(1);
        AddBuffer();

        break;
    case Type::Edge:
        m_VA = new VertexArray();
        m_VB = new VertexBuffer(MAX_EDGE_COUNT * sizeof(Edge));
        m_Layout = new VertexBufferLayout();
        indeces = new unsigned int[MAX_EDGE_INDEX_COUNT];
        InitQuadIndeces(indeces, MAX_EDGE_INDEX_COUNT);
        m_IB = new IndexBuffer(indeces, MAX_EDGE_INDEX_COUNT);
        m_Shader = new Shader(filepath);

        PushLayout<float>(2);
        PushLayout<float>(2);
        AddBuffer();

        break;
    default:
        break;
    }

}

QuadRenderer::~QuadRenderer(){
    delete m_VA;
	delete m_VB;
	delete m_Layout;
	delete m_IB;
	delete m_Shader;
}

void QuadRenderer::AddBuffer(){
    m_VA->AddBuffer(*m_VB, *m_Layout);
}

void QuadRenderer::SetUniform1f(const std::string& name, float value){
    m_Shader->SetUniform1f(name, value);
}

void QuadRenderer::SetUniform3f(const std::string& name, float v0, float v1, float v2){
    m_Shader->SetUniform3f(name, v0, v1, v2);
}

void QuadRenderer::SetUniformMat4f(const std::string& name, const glm::mat4& matrix){
    m_Shader->SetUniformMat4f(name, matrix);
}

void QuadRenderer::Bind(){
    m_Shader->Bind();
	m_VA->Bind();
	m_IB->Bind();
}

void QuadRenderer::Draw(const void* data, unsigned int elementSize, unsigned int indexNumber) {
    m_VB->DynamicBufferSubData(data, elementSize);
    glDrawElements(GL_TRIANGLES, indexNumber, GL_UNSIGNED_INT, nullptr);
}

template<typename T>
void QuadRenderer::PushLayout(unsigned int count) {
    static_assert(false);
}

template<>
void QuadRenderer::PushLayout<float>(unsigned int count) {
    m_Layout->Push<float>(count);
}