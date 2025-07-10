#include "../header-file/read-table.hpp"

#include <iostream>
#include <fstream> 
#include <string>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

#include "../../environment/env.cpp"

using namespace std;

TableStructure ReadTable(const string &tableName) {
    ENV env;
    
    string folder = env.TABLE_DATA_STORE_FOLDER;
    ifstream in(folder + "/" + tableName + ".bin", ios::binary);
    if (!in) {
        cerr << "Failed to open file: " << tableName << endl;
        return TableStructure(); 
    }

    TableStructure table;

    // Read table name
    size_t name_len;
    in.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));

    table.table_name.resize(name_len);
    in.read(&table.table_name[0], name_len);

    // Read column count
    in.read(reinterpret_cast<char*>(&table.table_columns), sizeof(table.table_columns));

    // Read column data
    for (int i = 0; i < table.table_columns; ++i) {
        ColumnStructure col;

        // Read column name
        size_t col_name_len;
        in.read(reinterpret_cast<char*>(&col_name_len), sizeof(col_name_len));
        col.column_name.resize(col_name_len);
        in.read(&col.column_name[0], col_name_len);

        // Read column type
        size_t col_type_len;
        in.read(reinterpret_cast<char*>(&col_type_len), sizeof(col_type_len));
        col.column_type.resize(col_type_len);
        in.read(&col.column_type[0], col_type_len);

        table.table_column_info.push_back(col);
    }

    cout<<table.table_name<<endl;
    cout<<table.table_columns<<endl;
    for (const auto& col : table.table_column_info) {
        cout<<col.column_name<<endl;
        cout<<col.column_type<<endl;
    }

    in.close();
    return table;
}
