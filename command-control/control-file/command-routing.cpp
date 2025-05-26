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

        if (command == "CREATE") {
            bool got_all_info_to_create_a_table = true;
            string table_name = "";
            int table_columns = 0;

            // Ask for table name
            const char* namePrompt = "Table Name: ";
            send(clientSocket, namePrompt, strlen(namePrompt), 0);

            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string name(buffer);
                name.erase(name.find_last_not_of(" \n\r\t") + 1);
                table_name = name;

                // Ask for column count
                const char* colPrompt = "Table Columns: ";
                send(clientSocket, colPrompt, strlen(colPrompt), 0);

                bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    string columnStr(buffer);
                    columnStr.erase(columnStr.find_last_not_of(" \n\r\t") + 1);

                    try {
                        table_columns = stoi(columnStr);
                    } catch (...) {
                        got_all_info_to_create_a_table = false;
                        const char* err = "Invalid column count.\n";
                        send(clientSocket, err, strlen(err), 0);
                    }
                } else {
                    got_all_info_to_create_a_table = false;
                    const char* err = "No column count received.\n";
                    send(clientSocket, err, strlen(err), 0);
                }
            } else {
                got_all_info_to_create_a_table = false;
                const char* err = "No table name received.\n";
                send(clientSocket, err, strlen(err), 0);
            }

            // Final response
            if (got_all_info_to_create_a_table) {
                string response = "A Nice table [" + table_name + "] is created with [" + to_string(table_columns) + "] columns.\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            } else {
                const char* response = "Didn't get all necessary data to create a nice table.\n";
                send(clientSocket, response, strlen(response), 0);
            }
        }
        else if (command == "EXIT") {
            const char* response = "Closing connection... Goodbye! :)\n";
            send(clientSocket, response, strlen(response), 0);
            break;
        }
        else {
            const char* response = "Invalid command. Available commands: [CREATE, EXIT]\n";
            send(clientSocket, response, strlen(response), 0);
        }
    }
}