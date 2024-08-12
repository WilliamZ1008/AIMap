#include "ElementCoordinator.h"


ElementCoordinator::ElementCoordinator(){
	DataManager data_manager("res/data/aimap.db");

	data_manager.LoadElements();

	m_Element_Number = data_manager.GetElementNumber();
	m_Edge_Number = data_manager.GetEdgeNumber();
	m_Elements = data_manager.GetElements();
	m_Edges = data_manager.GetEdges();

	// Initialize selected vertex
	m_Selected_Vertex = MAX_QUAD_COUNT;
}

ElementCoordinator::~ElementCoordinator(){

}

bool ElementCoordinator::CheckEdge(unsigned int vertex_1, unsigned int vertex_2){
	return m_Edges[vertex_1].find(vertex_2)!= m_Edges[vertex_1].end();
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
			if (CheckEdge(i, j)) {
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

			if (m_Elements[i].t) {
				m_Elements[i].x += c4 * force_x;
				m_Elements[i].y += c4 * force_y;
			}
			m_Elements[j].x -= c4 * force_x;
			m_Elements[j].y -= c4 * force_y;
		}
	}
}



void ElementCoordinator::OnUpdate(){
	Spring();
}

unsigned int ElementCoordinator::GetGLVertexNumber(){
	return m_Element_Number * 4;
}

unsigned int ElementCoordinator::GetGLEdgeNumber(){
	return m_Edge_Number * 4;
}

unsigned int ElementCoordinator::GetGLVertexSize(){
	return m_Element_Number * 4 * sizeof(Vertex);
}

unsigned int ElementCoordinator::GetGLEdgeSize(){
	return m_Edge_Number * 4 * sizeof(Edge);
}

unsigned int ElementCoordinator::GetGLVertexIndexNumber(){
	return m_Element_Number * 6;
}

unsigned int ElementCoordinator::GetGLEdgeIndexNumber(){
	return m_Edge_Number * 6;
}

std::array<Vertex, MAX_VERTEX_COUNT> ElementCoordinator::GetGLVertices(){	
	std::array<Vertex, MAX_VERTEX_COUNT> vertices;
	Vertex* target = vertices.data();

	for (size_t i = 0; i < m_Element_Number; i++){
		switch (m_Elements[i].t) {
		case ROOT:
			target = CreateQuad(target, m_Elements[i].x, m_Elements[i].y, 50.0f, (float)i==m_Selected_Vertex);
			break;
		case FIELD:
			target = CreateQuad(target, m_Elements[i].x, m_Elements[i].y, 30.9f, (float)i==m_Selected_Vertex);
			break;
		case SUB_FIELD:
			target = CreateQuad(target, m_Elements[i].x, m_Elements[i].y, 19.1f, (float)i==m_Selected_Vertex);
			break;
		case MODEL:
			target = CreateQuad(target, m_Elements[i].x, m_Elements[i].y, 11.8f, (float)i==m_Selected_Vertex);
			break;
		}
		
	}
	return vertices;
}

std::array<Edge, MAX_EDGE_COUNT> ElementCoordinator::GetGLEdges(){
	std::array<Edge, MAX_EDGE_COUNT> edges;
	Edge* target = edges.data();

	for (size_t i = 0; i < m_Element_Number; i++){
		for (size_t j = i + 1; j < m_Element_Number; j++) {
			if (CheckEdge(i, j)) {
				target = CreateEdge(target, m_Elements[i].x, m_Elements[i].y, m_Elements[j].x, m_Elements[j].y, 5.0f);
			}
		}
	}

	return edges;
}

void ElementCoordinator::SelectElementByCoord(glm::vec4 coord){
	float distanceMin = MAX_SELECT_DISTANCE_SQUARED;
	float distanceEach;

	unsigned int selected = m_Element_Number;

	for (size_t i = 0; i < m_Element_Number; i++) {
		distanceEach = pow((m_Elements[i].x - coord.x), 2) + pow((m_Elements[i].y - coord.y), 2);
		if (distanceEach < MAX_SELECT_DISTANCE_SQUARED && distanceEach < distanceMin) {
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
	return m_Selected_Vertex!= MAX_QUAD_COUNT;
}
