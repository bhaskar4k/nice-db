#include "./message-transfer-via-socket.hpp"

#include <iostream>
#include <string>

#ifdef _WIN32 
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#else 
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

using namespace std;

bool SafeReceiveMessage(SOCKET &sock, char *buffer, int bufsize, int &bytesRead) {
    bytesRead = recv(sock, buffer, bufsize - 1, 0);
    if (bytesRead == 0 || bytesRead == SOCKET_ERROR) {
        return false;
    } 
    
    buffer[bytesRead] = '\0';
    return true;
}

void PrintEmptyLine(SOCKET &clientSocket) {
    string message = "\n";
    SendMessage(clientSocket, message);
}

void SendMessage(SOCKET &clientSocket, string message) {
    send(clientSocket, message.c_str(), message.length(), 0);
}