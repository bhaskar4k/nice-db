#ifndef MESSAGE_TRANSFER_VIA_SOCKET_HEADER
#define MESSAGE_TRANSFER_VIA_SOCKET_HEADER

#include <string>
using namespace std;

#ifdef _WIN32
    #include <winsock2.h>
#else 
    #include <sys/socket.h> 
#endif

bool SafeReceiveMessage(SOCKET &sock, char *buffer, int bufsize, int &bytesRead);
void PrintEmptyLine(SOCKET &clientSocket);
void SendMessage(SOCKET &clientSocket, string message);

#endif
