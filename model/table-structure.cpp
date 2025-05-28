#include <iostream>
#include <string>
#include <vector>

#include "../model/column-structure.cpp"

using namespace std;

class TableStructure {
public:
    string table_name;
    int table_columns;
    vector<ColumnStructure> table_column_info;

    TableStructure(string name, int columns) {
        this->table_name = name;
        this->table_columns = columns;
        this->table_column_info.resize(columns);
    }
};