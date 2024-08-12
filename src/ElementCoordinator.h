#pragma once

#include <array>
#include <set>
#include <math.h>

#include "DataManager.h"
#include "glm/glm.hpp"
#include "Structs.h"
#include "Config.h"
#include "Utils.h"

#include <iostream>

class ElementCoordinator {
private:
	unsigned int m_Element_Number;
	unsigned int m_Edge_Number;
	unsigned int m_Selected_Vertex;
	std::array<Element, MAX_QUAD_COUNT> m_Elements;
	std::array<std::set<unsigned int>, MAX_QUAD_COUNT> m_Edges;
	void Spring(float c1 = 2, float c2 = 100, float c3 = 100000, float c4 = 1);

public:
	ElementCoordinator();
	~ElementCoordinator();

	bool CheckEdge(unsigned int vertex_1, unsigned int vertex_2);

	void OnUpdate();
	unsigned int GetGLVertexNumber();
	unsigned int GetGLEdgeNumber();
	unsigned int GetGLVertexSize();
	unsigned int GetGLEdgeSize();
	unsigned int GetGLVertexIndexNumber();
	unsigned int GetGLEdgeIndexNumber();
	std::array<Vertex, MAX_VERTEX_COUNT> GetGLVertices();
	std::array<Edge, MAX_EDGE_COUNT> GetGLEdges();

	void SelectElementByCoord(glm::vec4 coord);
	void MoveSelectedElement(glm::vec4 coord);
	bool ElementSelected();
};