#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <sys/stat.h>

namespace fs = std::filesystem;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// ANSI color codes
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

// Split input into tokens
vector<string> split_input(const string &input)
{
    vector<string> tokens;
    std::istringstream iss(input);
    string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

// Print prompt
void print_prompt(const fs::path &currentPath)
{
    cout << BLUE << currentPath.string() << ">" << RESET << " ";
}

// Print help
void print_help()
{
    cout << "Available commands:" << endl;
    cout << " help                 show this help" << endl;
    cout << " exit                 exit the program" << endl;
    cout << " ls                   list files/folders" << endl;
    cout << " cd <dir>             change directory" << endl;
    cout << " mkdir <dir>          create directory" << endl;
    cout << " create <file>        create file" << endl;
    cout << " rm <file/dir>        remove file or empty directory" << endl;
    cout << " cp / copy <src> <dest>      copy file or directory" << endl;
    cout << " mv / move <src> <dest>      move/rename file or directory" << endl;
    cout << " info <file/dir>      show details of file/folder" << endl;
    cout << " perm <file/dir>      show file permissions" << endl;
    cout << " search <name>        search file/folder recursively" << endl;
    cout << " pwd                  print current directory" << endl;
}

// Print info about a file/folder (date only)
void print_info(const fs::path &path)
{
    try
    {
        if (!fs::exists(path))
        {
            cout << RED << "info: does not exist: " << path.string() << RESET << endl;
            return;
        }

        cout << "Name: " << path.filename().string() << endl;
        cout << "Type: " << (fs::is_directory(path) ? "Directory" : "File") << endl;
        if (!fs::is_directory(path))
        {
            cout << "Size: " << fs::file_size(path) << " bytes" << endl;
        }

        auto ftime = fs::last_write_time(path);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        std::tm *lt = std::localtime(&cftime);
        cout << "Last Modified: " << std::put_time(lt, "%a %b %d %Y") << endl;
    }
    catch (const std::exception &e)
    {
        cout << RED << "info: error: " << e.what() << RESET << endl;
    }
}

// Recursive copy helper
void copy_recursive(const fs::path &src, const fs::path &dest)
{
    if (fs::is_directory(src))
    {
        fs::create_directories(dest);
        for (const auto &entry : fs::directory_iterator(src))
        {
            copy_recursive(entry.path(), dest / entry.path().filename());
        }
    }
    else
    {
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
    }
}

// Recursive search helper
void search_recursive(const fs::path &dir, const string &name)
{
    try
    {
        for (const auto &entry : fs::recursive_directory_iterator(dir))
        {
            if (entry.path().filename().string() == name)
            {
                if (entry.is_directory())
                    cout << YELLOW << entry.path().string() << "/" << RESET << endl;
                else
                    cout << GREEN << entry.path().string() << RESET << endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        cout << RED << "search: error: " << e.what() << RESET << endl;
    }
}

// Show file permissions
void show_permissions(const fs::path &path)
{
    if (!fs::exists(path))
    {
        cout << RED << "perm: does not exist: " << path.string() << RESET << endl;
        return;
    }

    std::error_code ec;
    fs::perms p = fs::status(path, ec).permissions();
    if (ec)
    {
        cout << RED << "perm: error reading permissions: " << ec.message() << RESET << endl;
        return;
    }

    auto check = [](fs::perms perm)
    { return (perm != fs::perms::none) ? GREEN + string("rwx") + RESET : RED + string("---") + RESET; };

    cout << "Permissions (rwx for owner, group, others):" << endl;
    cout << "Owner: ";
    cout << ((p & fs::perms::owner_read) != fs::perms::none ? GREEN + "r" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::owner_write) != fs::perms::none ? GREEN + "w" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::owner_exec) != fs::perms::none ? GREEN + "x" + RESET : RED + "-" + RESET) << endl;

    cout << "Group: ";
    cout << ((p & fs::perms::group_read) != fs::perms::none ? GREEN + "r" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::group_write) != fs::perms::none ? GREEN + "w" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::group_exec) != fs::perms::none ? GREEN + "x" + RESET : RED + "-" + RESET) << endl;

    cout << "Others: ";
    cout << ((p & fs::perms::others_read) != fs::perms::none ? GREEN + "r" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::others_write) != fs::perms::none ? GREEN + "w" + RESET : RED + "-" + RESET);
    cout << ((p & fs::perms::others_exec) != fs::perms::none ? GREEN + "x" + RESET : RED + "-" + RESET) << endl;
}

int main()
{
    fs::path currentPath = fs::current_path();
    cout << "File explorer CLI (C++) - Skeleton" << endl;
    cout << "Starting in: " << currentPath.string() << endl;
    cout << "Type 'help' for commands." << endl
         << endl;

    while (true)
    {
        print_prompt(currentPath);

        string line;
        if (!std::getline(cin, line))
        {
            cout << endl
                 << "Input closed. Exiting." << endl;
            break;
        }
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.find_first_not_of(" \t\r\n") == string::npos)
            continue;

        vector<string> tokens = split_input(line);
        if (tokens.empty())
            continue;

        string cmd = tokens[0];
        for (char &c : cmd)
            c = std::tolower(c);

        // ---------- Built-in Commands ----------
        if (cmd == "help")
        {
            print_help();
            continue;
        }
        if (cmd == "exit")
        {
            cout << "Exiting File explorer. Goodbye!" << endl;
            break;
        }

        // ---------- ls ----------
        if (cmd == "ls")
        {
            try
            {
                for (const auto &entry : fs::directory_iterator(currentPath))
                {
                    if (entry.is_directory())
                        cout << CYAN << entry.path().filename().string() << "/" << RESET << endl;
                    else
                        cout << entry.path().filename().string() << endl;
                }
            }
            catch (const std::exception &e)
            {
                cout << RED << "ls: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- cd ----------
        if (cmd == "cd")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: cd <directory>" << endl;
                continue;
            }
            fs::path newPath = (tokens[1] == "..") ? currentPath.parent_path() : (fs::path(tokens[1]).is_absolute() ? fs::path(tokens[1]) : currentPath / tokens[1]);
            if (!fs::exists(newPath))
            {
                cout << RED << "cd: directory does not exist: " << newPath.string() << RESET << endl;
                continue;
            }
            if (!fs::is_directory(newPath))
            {
                cout << RED << "cd: not a directory: " << newPath.string() << RESET << endl;
                continue;
            }
            currentPath = fs::canonical(newPath);
            continue;
        }

        // ---------- mkdir ----------
        if (cmd == "mkdir")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: mkdir <foldername>" << endl;
                continue;
            }
            fs::path newDir = currentPath / tokens[1];
            try
            {
                if (fs::exists(newDir))
                    cout << RED << "mkdir: directory already exists: " << newDir.string() << RESET << endl;
                else
                {
                    fs::create_directories(newDir);
                    cout << GREEN << "Directory created: " << newDir.string() << RESET << endl;
                }
            }
            catch (const std::exception &e)
            {
                cout << RED << "mkdir: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- create ----------
        if (cmd == "create")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: create <filename>" << endl;
                continue;
            }
            fs::path filePath = currentPath / tokens[1];
            try
            {
                if (fs::exists(filePath))
                    cout << RED << "create: file already exists: " << filePath.string() << RESET << endl;
                else
                {
                    std::ofstream ofs(filePath);
                    cout << GREEN << "File created: " << filePath.string() << RESET << endl;
                }
            }
            catch (const std::exception &e)
            {
                cout << RED << "create: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- rm ----------
        if (cmd == "rm")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: rm <file/dir>" << endl;
                continue;
            }
            fs::path targetPath = currentPath / tokens[1];
            try
            {
                if (!fs::exists(targetPath))
                    cout << RED << "rm: does not exist: " << targetPath.string() << RESET << endl;
                else if (fs::is_directory(targetPath) && !fs::is_empty(targetPath))
                    cout << RED << "rm: directory not empty: " << targetPath.string() << RESET << endl;
                else
                {
                    fs::remove(targetPath);
                    cout << GREEN << (fs::is_directory(targetPath) ? "Directory removed: " : "File removed: ") << targetPath.string() << RESET << endl;
                }
            }
            catch (const std::exception &e)
            {
                cout << RED << "rm: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- cp / copy ----------
        if (cmd == "cp" || cmd == "copy")
        {
            if (tokens.size() < 3)
            {
                cout << "Usage: cp/copy <source> <destination>" << endl;
                continue;
            }
            fs::path src = currentPath / tokens[1];
            fs::path dest = currentPath / tokens[2];
            try
            {
                if (!fs::exists(src))
                {
                    cout << RED << "cp: source does not exist: " << src.string() << RESET << endl;
                    continue;
                }
                copy_recursive(src, dest);
                cout << GREEN << "Copied " << src.string() << " -> " << dest.string() << RESET << endl;
            }
            catch (const std::exception &e)
            {
                cout << RED << "cp: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- mv / move ----------
        if (cmd == "mv" || cmd == "move")
        {
            if (tokens.size() < 3)
            {
                cout << "Usage: mv/move <source> <destination>" << endl;
                continue;
            }
            fs::path src = currentPath / tokens[1];
            fs::path dest = currentPath / tokens[2];
            try
            {
                if (!fs::exists(src))
                {
                    cout << RED << "mv: source does not exist: " << src.string() << RESET << endl;
                    continue;
                }
                // If destination is a directory, move inside it
                if (fs::exists(dest) && fs::is_directory(dest))
                    dest /= src.filename();

                fs::rename(src, dest);
                cout << GREEN << "Moved " << src.string() << " -> " << dest.string() << RESET << endl;
            }
            catch (const std::exception &e)
            {
                cout << RED << "mv: error: " << e.what() << RESET << endl;
            }
            continue;
        }

        // ---------- info ----------
        if (cmd == "info")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: info <file/dir>" << endl;
                continue;
            }
            fs::path target = currentPath / tokens[1];
            print_info(target);
            continue;
        }

        // ---------- perm ----------
        if (cmd == "perm")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: perm <file/dir>" << endl;
                continue;
            }
            fs::path target = currentPath / tokens[1];
            show_permissions(target);
            continue;
        }

        // ---------- search ----------
        if (cmd == "search")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: search <name>" << endl;
                continue;
            }
            search_recursive(currentPath, tokens[1]);
            continue;
        }

        // ---------- pwd ----------
        if (cmd == "pwd")
        {
            cout << currentPath.string() << endl;
            continue;
        }

        // ---------- Fallback ----------
        cout << "Command received: " << cmd << endl;
        if (tokens.size() > 1)
        {
            cout << "Arguments:";
            for (size_t i = 1; i < tokens.size(); ++i)
                cout << "[" << tokens[i] << "]";
            cout << endl;
        }
        else
            cout << "No arguments." << endl;
        cout << "(Command not implemented yet - continue)" << endl
             << endl;
    }

    return EXIT_SUCCESS;
}
