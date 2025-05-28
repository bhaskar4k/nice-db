#include <iostream>
#include <string>

#include "../model/column-structure.hpp"

using namespace std;

ColumnStructure::ColumnStructure() {}

ColumnStructure::ColumnStructure(string name, string type) {
    this->column_name = name;
    this->column_type = type;
}