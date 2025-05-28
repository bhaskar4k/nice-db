#ifndef CREATE_TABLE_HEADER
#define CREATE_TABLE_HEADER

#include <string>

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

#include "../../model/table-structure.hpp"
using namespace std;

bool BuildTable(TableStructure table);

#endif
