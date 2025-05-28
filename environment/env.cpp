#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ENV {
public:
    const int PORT = 69;
    const vector<string> AVAILABLE_COMMANDS = {"COMMANDS", "CREATE", "EXIT"};

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