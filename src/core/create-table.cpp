#include "../header-file/create-table.hpp"

#include <windows.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

#include "../../environment/env.cpp"

using namespace std;

bool DirectoryExists(const string& path) {
    DWORD ftyp = GetFileAttributesA(path.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}

void CreateDirectoryIfNotExists(const string& path) {
    if (!DirectoryExists(path)) {
        CreateDirectoryA(path.c_str(), NULL);
    }
}

bool BuildTable(TableStructure table){
    ENV env;
    
    string folder = env.TABLE_DATA_STORE_FOLDER;
    CreateDirectoryIfNotExists(folder); 

    string filename = folder + "/" + table.table_name + ".bin";
    ofstream out(filename, ios::binary);
    if (!out) {
        cerr << "Failed to open file: " << filename << endl;
        return false;
    }

    size_t name_len = table.table_name.size();
    out.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
    out.write(table.table_name.c_str(), name_len);

    out.write(reinterpret_cast<const char*>(&table.table_columns), sizeof(table.table_columns));

    for (const auto& col : table.table_column_info) {
        size_t col_name_len = col.column_name.size();
        out.write(reinterpret_cast<const char*>(&col_name_len), sizeof(col_name_len));
        out.write(col.column_name.c_str(), col_name_len);

        size_t col_type_len = col.column_type.size();
        out.write(reinterpret_cast<const char*>(&col_type_len), sizeof(col_type_len));
        out.write(col.column_type.c_str(), col_type_len);
    }

    out.close();
    return true;
}