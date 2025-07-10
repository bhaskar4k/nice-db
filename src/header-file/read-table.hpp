#ifndef READ_TABLE_HEADER
#define READ_TABLE_HEADER

#include <string>

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

#include "../../model/table-structure.hpp"
using namespace std;

TableStructure ReadTable(const string& filename);

#endif
