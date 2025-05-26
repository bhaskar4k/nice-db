#include "../header-file/command-routing.hpp"

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

void HandleCommand(SOCKET &clientSocket) {
    char buffer[1024];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        string command(buffer);
        command.erase(command.find_last_not_of(" \n\r\t") + 1);

        if (command == "SAY_HELLO") {
            const char* prompt = "Enter your name: ";
            send(clientSocket, prompt, strlen(prompt), 0);

            // Receive the name from the client
            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string name(buffer);
                name.erase(name.find_last_not_of(" \n\r\t") + 1);

                string response = "Hello, " + name + "!\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            } else {
                const char* error = "No name received.\n";
                send(clientSocket, error, strlen(error), 0);
            }
        }
        else if (command == "EXIT") {
            const char* response = "Closing connection... Goodbye! :)\n";
            send(clientSocket, response, strlen(response), 0);
            break;
        }
        else {
            const char* response = "Unknown command\n";
            send(clientSocket, response, strlen(response), 0);
        }
    }
}