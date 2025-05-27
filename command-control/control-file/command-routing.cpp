#include "../header-file/command-routing.hpp"
#include "../header-file/create-table-routing.hpp"
#include "../../utils/message-transfer-via-socket.hpp"

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

#include "../../environment/env.cpp"

using namespace std;

void HandleCommand(SOCKET &clientSocket) {
    char buffer[1024];
    int bytesRead;
    string response;
    ENV env;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        string command(buffer);
        command.erase(command.find_last_not_of(" \n\r\t") + 1);

        if (command == "CREATE") {
            HandleCreateTableCommand(clientSocket);
        }
        else if (command == "EXIT") {
            response = "\n=========================================\n";
            response += "|   Closing connection... Goodbye! :)   |\n";
            response += "=========================================\n";
            SendMessage(clientSocket, response);
            break;
        }
        else {
            SendMessage(clientSocket, "Invalid command. " + env.GetAvailableCommandsAsString() + "\n");
        }
    }
}