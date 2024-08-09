#include "ElementCoordinator.h"


ElementCoordinator::ElementCoordinator(){
	m_Element_Number = 5;
	
	// Initialize elements
	m_Elements[0] = Element{100.5f, 350.6f};
	m_Elements[1] = Element{156.6f, 200.3f};
	m_Elements[2] = Element{50.9f, 586.8f};
	m_Elements[3] = Element{100.3f, 10.8f};
	m_Elements[4] = Element{150.9f, 566.3f};

	// Initialize edges
	for (size_t i = 0; i < m_Element_Number; i++){
		for (size_t j = 0; j < m_Element_Number; j++){
			m_Edges[i][j] = false;
		}

	}

	m_Edge_Number = 5;

	m_Edges[0][1] = true;
	m_Edges[1][0] = true;

	m_Edges[0][2] = true;
	m_Edges[2][0] = true;

	m_Edges[1][2] = true;
	m_Edges[2][2] = true;

	m_Edges[0][3] = true;
	m_Edges[3][0] = true;
	
	m_Edges[0][4] = true;
	m_Edges[4][0] = true;

	// Initialize selected vertex
	m_Selected_Vertex = m_Element_Number;

}

ElementCoordinator::~ElementCoordinator(){
}

void ElementCoordinator::Spring(float c1, float c2, float c3, float c4){
	float x1, x2, y1, y2, d, force, theta, force_x, force_y;
	for (size_t i = 0; i < m_Element_Number; i++){
		for (size_t j = i+1; j < m_Element_Number; j++){
			x1 = m_Elements[i].x;
			y1 = m_Elements[i].y;
			x2 = m_Elements[j].x;
			y2 = m_Elements[j].y;
			d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
			if (m_Edges[i][j]) {
				// Adjacent
				force = c1 * log(d / c2);
			}
			else {
				// Nonadjacent
				force = -c3 * pow(d, -2);
			}
			theta = atan2(y2 - y1, x2 - x1);
			force_x = force * cos(theta);
			force_y = force * sin(theta);

			m_Elements[i].x += c4 * force_x;
			m_Elements[j].x -= c4 * force_x;
			m_Elements[i].y += c4 * force_y;
			m_Elements[j].y -= c4 * force_y;
		}
	}
}



void ElementCoordinator::OnUpdate(){
	Spring();
}

unsigned int ElementCoordinator::getGLVertexNumber(){
	return m_Element_Number * 4;
}

unsigned int ElementCoordinator::getGLEdgeNumber(){
	return m_Edge_Number * 4;
}

unsigned int ElementCoordinator::getGLVertexIndexNumber(){
	return m_Element_Number * 6;
}

unsigned int ElementCoordinator::getGLEdgeIndexNumber(){
	return m_Edge_Number * 6;
}

std::array<Vertex, MAX_VERTEX_COUNT> ElementCoordinator::getGLVertices(){	
	std::array<Vertex, MAX_VERTEX_COUNT> vertices;
	Vertex* target = vertices.data();

	for (size_t i = 0; i < m_Element_Number; i++){
		target = CreateQuad(target, m_Elements[i].x, m_Elements[i].y, 50.0f, (float)i==m_Selected_Vertex);
	}
	return vertices;
}

std::array<Edge, MAX_EDGE_COUNT> ElementCoordinator::getGLEdges(){
	std::array<Edge, MAX_EDGE_COUNT> edges;
	Edge* target = edges.data();

	for (size_t i = 0; i < m_Element_Number; i++){
		for (size_t j = i + 1; j < m_Element_Number; j++) {
			if (m_Edges[i][j]) {
				target = CreateEdge(target, m_Elements[i].x, m_Elements[i].y, m_Elements[j].x, m_Elements[j].y, 5.0f);
			}
		}
	}

	return edges;
}

void ElementCoordinator::SelectElementByCoord(glm::vec4 coord){
	float distanceMin = 99999999.0;
	float distanceEach;

	unsigned int selected = m_Element_Number;

	for (size_t i = 0; i < m_Element_Number; i++) {
		distanceEach = pow((m_Elements[i].x - coord.x), 2) + pow((m_Elements[i].y - coord.y), 2);
		if (distanceEach < 1000 && distanceEach < distanceMin) {
			distanceMin = distanceEach;
			selected = i;
		}
	}

	m_Selected_Vertex = selected;
	//std::cout << m_Selected_Vertex << std::endl;
}

void ElementCoordinator::MoveSelectedElement(glm::vec4 coord) {
	m_Elements[m_Selected_Vertex].x = coord.x;
	m_Elements[m_Selected_Vertex].y = coord.y;
}

bool ElementCoordinator::ElementSelected()
{
	return m_Selected_Vertex!=m_Element_Number;
}
