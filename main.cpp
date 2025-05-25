#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h> 
#pragma comment(lib, "ws2_32.lib") 
#include <cstdlib>
#include <fstream>
#include <cstdlib>
#include <unordered_map>

std::unordered_map<std::string, std::string> parseEnvFile(const std::string& path) {
    std::unordered_map<std::string, std::string> envMap;
    std::ifstream envFile(path);
    std::string line;

    while (std::getline(envFile, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            envMap[key] = value;
        }
    }
    return envMap;
}

void handleCommand(SOCKET clientSocket, const std::string& command) {
    if (command == "SAY_HELLO") {
        std::cout << "Hello from server!" << std::endl;
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
    auto env = parseEnvFile(".env");
    int port = std::stoi(env["PORT"]);
    
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind and listen
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cout << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    // Accept a client
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Handle commands
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        std::string command(buffer);
        command.erase(command.find_last_not_of(" \n\r\t") + 1);  // Trim whitespace
        handleCommand(clientSocket, command);
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}