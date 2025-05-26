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
            
            if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0){
                const char* prompt = "Table Name : ";
                send(clientSocket, prompt, strlen(prompt), 0);

                bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    string name(buffer);
                    name.erase(name.find_last_not_of(" \n\r\t") + 1);
                    table_name = name;

                    const char* prompt = "Table Columns : ";
                    send(clientSocket, prompt, strlen(prompt), 0);

                    if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
                        buffer[bytesRead] = '\0';
                        string column(buffer);
                        column.erase(column.find_last_not_of(" \n\r\t") + 1);
                        table_columns = stoi(column);
                    } else {
                        const char* error = "No column count received.\n";
                        send(clientSocket, error, strlen(error), 0);
                        got_all_info_to_create_a_table = false;
                    } 
                } else {
                    const char* error = "No table name received.\n";
                    send(clientSocket, error, strlen(error), 0);
                    got_all_info_to_create_a_table = false;
                }

                if(got_all_info_to_create_a_table){
                    string response = "A Nice table [" + table_name + "] created with [" + to_string(table_columns) + "] columns.\n";
                    send(clientSocket, response.c_str(), response.length(), 0);
                }else{
                    string response = "Didn't get all necessery data to create a nice table.\n";
                    send(clientSocket, response.c_str(), response.length(), 0);
                }
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