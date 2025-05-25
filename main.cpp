#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h> 
#pragma comment(lib, "ws2_32.lib") 
#include <cstdlib>
#include <unordered_map>
#include "./environment/env.cpp"

using namespace std;

void handleCommand(SOCKET clientSocket, const string& command) {
    if (command == "SAY_HELLO") {
        cout << "Hello from server!" << endl;
        const char* response = "Hello executed!\n";
        send(clientSocket, response, strlen(response), 0);
    }
    else if (command == "EXIT") {
        const char* response = "Server shutting down...\n";
        send(clientSocket, response, strlen(response), 0);
        exit(0);
    }
    else {
        const char* response = "Unknown command\n";
        send(clientSocket, response, strlen(response), 0);
    }
}

int main() {
    int port = ENV::PORT;
    
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind and listen
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        cout << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << port << endl;

    // Accept a client
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Handle commands
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        string command(buffer);
        command.erase(command.find_last_not_of(" \n\r\t") + 1);  // Trim whitespace
        handleCommand(clientSocket, command);
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}