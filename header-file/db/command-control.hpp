#ifndef COMMAND_CONTROL_HEADER
#define COMMAND_CONTROL_HEADER

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

void HandleCommand(SOCKET &clientSocket);

#endif
