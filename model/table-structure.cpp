#include <iostream>
#include <string>
#include <vector>

#include "../model/table-structure.hpp"

using namespace std;

TableStructure::TableStructure() {}

TableStructure::TableStructure(string name, int columns) {
    this->table_name = name;
    this->table_columns = columns;
    this->table_column_info.resize(columns);
}