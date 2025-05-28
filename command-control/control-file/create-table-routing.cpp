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

#include "../../model/table-structure.cpp"

using namespace std;

void HandleCreateTableCommand(SOCKET &clientSocket){
    char buffer[1024];
    int bytesRead;
    string response;
    
    bool got_all_info_to_create_a_table = true;
    TableStructure new_table;

    SendMessage(clientSocket, "Table Name: ");
    bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        string name(buffer);
        name.erase(name.find_last_not_of(" \n\r\t") + 1);
        PrintEmptyLine(clientSocket);

        if (name.empty()) {
            got_all_info_to_create_a_table = false;
            SendMessage(clientSocket, "Table name cannot be empty.\n");
        } else {
            SendMessage(clientSocket, "Table Columns: ");
            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                string columnStr(buffer);
                columnStr.erase(columnStr.find_last_not_of(" \n\r\t") + 1);

                try {
                    int table_columns = stoi(columnStr);
                    new_table = TableStructure(name, table_columns);
                    PrintEmptyLine(clientSocket);

                    int i=0;
                    while (i < table_columns) {
                        SendMessage(clientSocket, "Column-" + to_string(i+1) + " Name: ");
                        bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

                        if (bytesRead > 0) {
                            buffer[bytesRead] = '\0';
                            string columnName(buffer);
                            columnName.erase(columnName.find_last_not_of(" \n\r\t") + 1);

                            SendMessage(clientSocket, "Column-" + to_string(i+1) + " Type: ");
                            bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

                            if (bytesRead > 0) {
                                buffer[bytesRead] = '\0';
                                string columnType(buffer);
                                columnType.erase(columnType.find_last_not_of(" \n\r\t") + 1);

                                if (columnType == "int" || columnType == "double" || columnType == "string" || columnType == "datetime") {
                                    new_table.table_column_info[i++] = ColumnStructure(columnName, columnType);
                                    PrintEmptyLine(clientSocket);
                                } else {
                                    SendMessage(clientSocket, "Invalid column type.\n");
                                }
                            } else {
                                SendMessage(clientSocket, "No column type recieved.\n");
                            }
                        } else {
                            SendMessage(clientSocket, "No column name recieved.\n");
                        }
                    }
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

    if (got_all_info_to_create_a_table) {
        response = "A Nice table [" + new_table.table_name + "] is created with [" + to_string(new_table.table_columns) + "] columns.\n";
        SendMessage(clientSocket, response);
    } else {
        SendMessage(clientSocket, "Didn't get all necessary data to create a nice table.\n");
    }
}