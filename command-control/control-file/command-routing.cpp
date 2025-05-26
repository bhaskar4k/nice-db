#include "../header-file/command-routing.hpp"
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
            SendMessage(clientSocket, "Table Name: ");

            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string name(buffer);
                name.erase(name.find_last_not_of(" \n\r\t") + 1);
                table_name = name;

                if(table_name.empty()) {
                    got_all_info_to_create_a_table = false;
                    SendMessage(clientSocket, "Table name cannot be empty.\n");
                }else{
                    // Ask for column count
                    SendMessage(clientSocket, "Table Columns: ");

                    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        string columnStr(buffer);
                        columnStr.erase(columnStr.find_last_not_of(" \n\r\t") + 1);

                        try {
                            table_columns = stoi(columnStr);
                        } catch (...) {
                            got_all_info_to_create_a_table = false;
                            SendMessage(clientSocket, "Invalid column count.\n");
                        }
                    } else {
                        got_all_info_to_create_a_table = false;
                        SendMessage(clientSocket, "No column count received.\n");
                    }
                }      
            } else {
                got_all_info_to_create_a_table = false;
                SendMessage(clientSocket, "No table name received.\n");
            }

            // Final response
            if (got_all_info_to_create_a_table) {
                response = "A Nice table [" + table_name + "] is created with [" + to_string(table_columns) + "] columns.\n";
                SendMessage(clientSocket, response);
            } else {
                SendMessage(clientSocket, "Didn't get all necessary data to create a nice table.\n");
            }
        }
        else if (command == "EXIT") {
            response = "\n=========================================\n";
            response += "|   Closing connection... Goodbye! :)   |\n";
            response += "=========================================\n";
            SendMessage(clientSocket, response);
            break;
        }
        else {
            SendMessage(clientSocket, "Invalid command. Available commands: [CREATE, EXIT]\n");
        }
    }
}