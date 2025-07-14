#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class ENV {
public:
    // Server
    const int PORT = 69;

    // Commands
    const vector<string> AVAILABLE_COMMANDS = {"COMMANDS", "CREATE", "EXIT"};

    // FileSystem
    const string TABLE_DATA_STORE_FOLDER = "tables";

    // DataType
    const set<string> DATA_TYPES{"int", "double", "string", "datetime", "bool", "char", "long", "long long", "unsigned short", "unsigned int", "unsigned long", "unsigned long long"};

    string GetAvailableCommandsAsString() {
        string response = "Available commands: [";
        for (size_t i = 0; i < AVAILABLE_COMMANDS.size(); ++i) {
            response += AVAILABLE_COMMANDS[i];
            if (i < AVAILABLE_COMMANDS.size() - 1) {
                response += ", ";
            }
        }

        response += "]";
        return response;
    }
};