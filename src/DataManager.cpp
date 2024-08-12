#include "DataManager.h"

DataManager::DataManager(const std::string& database_path)
	:m_DatabasePath(database_path), m_Database(nullptr), m_Elements(), m_Element_Number(0) {

}

DataManager::~DataManager(){
	Disconnect();
}

void DataManager::Connect(){
	sqlite3_open(m_DatabasePath.c_str(), &m_Database);
}

void DataManager::Disconnect(){
	sqlite3_close(m_Database);
}
