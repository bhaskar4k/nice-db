#ifndef COLUMN_STRUCTURE_HEADER
#define COLUMN_STRUCTURE_HEADER

#include <string>
using namespace std;

class ColumnStructure {
public:
    string column_name;
    string column_type;

    ColumnStructure();
    ColumnStructure(string name, string type);
};

#endif 
