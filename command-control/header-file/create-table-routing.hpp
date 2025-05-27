#ifndef COMMAND_CONTROL_CREATE_TABLE_HEADER
#define COMMAND_CONTROL_CREATE_TABLE_HEADER

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

void HandleCreateTableCommand(SOCKET &clientSocket);

#endif
