#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ENV {
public:
    // Server
    const int PORT = 69;

    // Commands
    const vector<string> AVAILABLE_COMMANDS = {"COMMANDS", "CREATE", "EXIT"};

    // FileSystem
    const string TABLE_DATA_STORE_FOLDER = "tables";

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