#pragma once
#include "sqlite3.h"

#include <string>
#include <array>

#include "Structs.h"
#include "Config.h"

class DataManager {
private:
	std::string m_DatabasePath;
	sqlite3* m_Database;

	unsigned int m_Element_Number;
	std::array<Element, MAX_QUAD_COUNT> m_Elements;
	unsigned int m_Edge_Number;
	std::array<std::set<unsigned int>, MAX_QUAD_COUNT> m_Edges;



public:
		DataManager(const std::string& database_path);
		~DataManager();

		void Connect();

		



		void Disconnect();

};