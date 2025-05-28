#include <iostream>
#include <string>

using namespace std;

class ColumnStructure {
public:
    string column_name;
    string column_type;

    ColumnStructure() {}

    ColumnStructure(string name, string type) {
        this->column_name = name;
        this->column_type = type;
    }
};