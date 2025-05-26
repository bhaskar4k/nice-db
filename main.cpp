#include <iostream>
#include <string>

#ifdef _WIN32 // For Win-like systems if needed
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib, "ws2_32.lib")
#else // For Unix-like systems if needed
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

#include "./environment/env.cpp"
#include "../header-file/db/command-control.hpp"

using namespace std;


int ConnectedClient = 0;
CRITICAL_SECTION cs;  // <-- WinAPI replacement for std::mutex

DWORD WINAPI handleClient(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;

    EnterCriticalSection(&cs);
    ConnectedClient++;
    cout << "Client connected. Concurrent connected clients: " << ConnectedClient << "\n";
    LeaveCriticalSection(&cs);

    const char* connectedResponse = "Connected to nice-db :)\n";
    send(clientSocket, connectedResponse, strlen(connectedResponse), 0);

    HandleCommand(clientSocket);

    closesocket(clientSocket);

    EnterCriticalSection(&cs);
    ConnectedClient--;
    cout << "Client disconnected. Concurrent connected clients: " << ConnectedClient << "\n";
    LeaveCriticalSection(&cs);

    return 0;
}

int main() {
    int port = ENV::PORT;

    InitializeCriticalSection(&cs);  // Initialize the critical section

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

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server listening on port " << port << endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Client accept failed: " << WSAGetLastError() << "\n";
            continue;
        }

        // Create a new thread using Windows API
        CreateThread(
            NULL,               // default security
            0,                  // default stack size
            handleClient,       // thread function
            (LPVOID)clientSocket, // parameter to thread function
            0,                  // default creation flags
            NULL                // no thread identifier
        );
    }

    DeleteCriticalSection(&cs);  // Clean up
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
