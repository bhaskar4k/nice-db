#ifndef TABLE_STRUCTURE_HEADER
#define TABLE_STRUCTURE_HEADER

#include <string>
#include <vector>
using namespace std;

#include "./column-structure.hpp"

class TableStructure {
public:
    string table_name;
    int table_columns;
    vector<ColumnStructure> table_column_info;

    TableStructure();
    TableStructure(string name, int columns);
};

#endif 
