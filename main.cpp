#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

namespace fs = std::filesystem;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// utility split line into tokens
vector<string> split_input(const string &input)
{
    vector<string> tokens;
    std::istringstream iss(input);
    string token;
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    return tokens;
}

// utility print prompt
void print_prompt(const fs::path &currentPath)
{
    cout << currentPath.string() << "> ";
}

// print help message
void print_help()
{
    cout << "Available commands (skeleton):" << endl;
    cout << " help                 show this help" << endl;
    cout << " exit                 exit the program" << endl;
    cout << " ls , cd , mkdir, create, rm , open, move , info (to be implemented)" << endl;
}

int main()
{

    // start at current working directory
    fs::path currentPath = fs::current_path();

    cout << "File explorer CLI(c++) - Skeleton" << endl;
    cout << "Starting in: " << currentPath.string() << endl;
    cout << "Type 'help' for commands. " << endl
         << endl;

    while (true)
    {
        // print prompt
        print_prompt(currentPath);

        // Read user input
        string line;
        if (!std::getline(cin, line))
        {
            // eof or error : break
            cout << endl
                 << "Input closed. Exiting." << endl;
            break;
        }

        // Trim leading/trailing whitespace
        if (line.find_first_not_of(" \t\r\n") == string::npos)
        {
            // empty line - continue
            continue;
        }

        vector<string> tokens = split_input(line);
        if (tokens.empty())
            continue;

        string cmd = tokens[0];

        // convert command to lower-case
        for (char &c : cmd)
            c = std::tolower(c);

        // handle built-in commands
        if (cmd == "help")
        {
            print_help();
            continue;
        }

        if (cmd == "exit")
        {
            cout << "Exiting File explorer, Goodbye!" << endl;
            break;
        }

        // Placeholder: dispatch table
        cout << "Command received: " << cmd << endl;

        if (tokens.size() > 1)
        {
            cout << "Arguments:";
            for (size_t i = 1; i < tokens.size(); ++i)
            {
                cout << "[" << tokens[i] << "]";
            }
            cout << endl;
        }
        else
        {
            cout << "No arguments." << endl;
        }

        cout << "(Command not implemented yet - continue)" << endl
             << endl;
    }

    return EXIT_SUCCESS;
}
