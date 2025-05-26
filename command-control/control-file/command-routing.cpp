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
    string response;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        string command(buffer);
        command.erase(command.find_last_not_of(" \n\r\t") + 1);

        if (command == "CREATE") {
            bool got_all_info_to_create_a_table = true;
            string table_name = "";
            int table_columns = 0;

            // Ask for table name
            response = "Table Name: ";
            send(clientSocket, response.c_str(), response.length(), 0);

            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string name(buffer);
                name.erase(name.find_last_not_of(" \n\r\t") + 1);
                table_name = name;

                if(table_name.empty()) {
                    got_all_info_to_create_a_table = false;
                    response = "Table name cannot be empty.\n";
                    send(clientSocket, response.c_str(), response.length(), 0);
                }else{
                    // Ask for column count
                    response = "Table Columns: ";
                    send(clientSocket, response.c_str(), response.length(), 0);

                    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        string columnStr(buffer);
                        columnStr.erase(columnStr.find_last_not_of(" \n\r\t") + 1);

                        try {
                            table_columns = stoi(columnStr);
                        } catch (...) {
                            got_all_info_to_create_a_table = false;
                            response = "Invalid column count.\n";
                            send(clientSocket, response.c_str(), response.length(), 0);
                        }
                    } else {
                        got_all_info_to_create_a_table = false;
                        response = "No column count received.\n";
                        send(clientSocket, response.c_str(), response.length(), 0);
                    }
                }      
            } else {
                got_all_info_to_create_a_table = false;
                response = "No table name received.\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            }

            // Final response
            if (got_all_info_to_create_a_table) {
                response = "A Nice table [" + table_name + "] is created with [" + to_string(table_columns) + "] columns.\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            } else {
                response = "Didn't get all necessary data to create a nice table.\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            }
        }
        else if (command == "EXIT") {
            response = "\n=========================================\n|   Closing connection... Goodbye! :)   |\n=========================================\n";
            send(clientSocket, response.c_str(), response.length(), 0);
            break;
        }
        else {
            response = "Invalid command. Available commands: [CREATE, EXIT]\n";
            send(clientSocket, response.c_str(), response.length(), 0);
        }
    }
}