#pragma once
#include "sqlite3.h"

#include <iostream>
#include <string>
#include <array>
#include <set>

#include "Structs.h"
#include "Config.h"

class DataManager {
private:
	std::string m_DatabasePath;
	sqlite3* m_Database;

	unsigned int m_Element_Number;
	std::array<Element, MAX_QUAD_COUNT> m_Elements;
	std::array<ElementInfo, MAX_QUAD_COUNT> m_Element_Infomation;
	unsigned int m_Edge_Number;
	std::array<std::set<unsigned int>, MAX_QUAD_COUNT> m_Edges;



public:
		DataManager(const std::string& database_path);
		~DataManager();

		void Connect();
		void Disconnect();

		void LoadElements();

		void AddEdge(unsigned int vertex_1, unsigned int vertex_2);

		std::array<Element, MAX_QUAD_COUNT> GetElements();
		std::array<ElementInfo, MAX_QUAD_COUNT> GetElementInfomation();
		std::array<std::set<unsigned int>, MAX_QUAD_COUNT> GetEdges();
		unsigned int GetElementNumber();
		unsigned int GetEdgeNumber();
};