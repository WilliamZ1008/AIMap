#pragma once

#include <array>
#include <math.h>

#include "Structs.h"
#include "Config.h"
#include "Utils.h"

class ElementCoordinator {
private:
	unsigned int m_Element_Number;
	unsigned int m_Edge_Number;
	std::array<Element, MAX_QUAD_COUNT> m_Elements;
	std::array<std::array<bool, MAX_QUAD_COUNT>, MAX_QUAD_COUNT> m_Edges;
	void Spring(float c1 = 2, float c2 = 100, float c3 = 100000, float c4 = 1);

public:
	ElementCoordinator();
	~ElementCoordinator();

	void OnUpdate();
	unsigned int getGLVertexNumber();
	unsigned int getGLEdgeNumber();
	unsigned int getGLVertexIndexNumber();
	unsigned int getGLEdgeIndexNumber();
	std::array<Vertex, MAX_VERTEX_COUNT> getGLVertices();
	std::array<Edge, MAX_EDGE_COUNT> getGLEdges();

};