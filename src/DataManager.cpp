#include "DataManager.h"

#define ASSERT(x) if (!(x)) __debugbreak();

DataManager::DataManager(const std::string& database_path)
	:m_DatabasePath(database_path), m_Database(nullptr), m_Element_Number(0), m_Elements(), m_Element_Infomation(), m_Edge_Number(0), m_Edges() {

    int rc = sqlite3_open(m_DatabasePath.c_str(), &m_Database);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_Database));
		ASSERT(false);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
    }

}

DataManager::~DataManager(){
	Disconnect();
}

void DataManager::Connect(){
	sqlite3_open(m_DatabasePath.c_str(), &m_Database);
}

void DataManager::Disconnect() {
	sqlite3_close(m_Database);
}

void DataManager::LoadElements(){
	const char* sql = "SELECT * FROM t_element";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_Database, sql, -1, &stmt, NULL);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		int type = sqlite3_column_int(stmt, 1);
		float x = sqlite3_column_double(stmt, 2);
		float y = sqlite3_column_double(stmt, 3);
		std::string en_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
		std::string cn_name = (char*)sqlite3_column_text(stmt, 5);
		std::string abbreviation = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
		std::string en_introduction = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
		std::string cn_introduction = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
		int parent_id = sqlite3_column_int(stmt, 9);

		Element element = { x, y, type };
		ElementInfo element_info = { en_name, cn_name, abbreviation, en_introduction, cn_introduction };

		m_Elements[m_Element_Number] = element;
		m_Element_Infomation[m_Element_Number] = element_info;
		m_Element_Number++;

		if (parent_id != -1) {
			AddEdge(parent_id, id);
			m_Edge_Number++;
		}

		//printf("id: %d, type: %f, x: %f, y: %f, en_name: %s, cn_name: %s, abbreviation: %s, parent_id: %d\n", id, type, x, y, en_name.c_str(), cn_name.c_str(), abbreviation.c_str(), parent_id);
	}
	sqlite3_finalize(stmt);
}

void DataManager::AddEdge(unsigned int vertex_1, unsigned int vertex_2) {
	m_Edges[vertex_1].insert(vertex_2);
	m_Edges[vertex_2].insert(vertex_1);
}




std::array<Element, MAX_QUAD_COUNT> DataManager::GetElements(){
	return m_Elements;
}

std::array<ElementInfo, MAX_QUAD_COUNT> DataManager::GetElementInfomation(){
	return m_Element_Infomation;
}

std::array<std::set<unsigned int>, MAX_QUAD_COUNT> DataManager::GetEdges(){
	return m_Edges;
}

unsigned int DataManager::GetElementNumber(){
	return m_Element_Number;
}

unsigned int DataManager::GetEdgeNumber(){
	return m_Edge_Number;
}
