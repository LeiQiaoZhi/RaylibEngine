#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>  // For system()
#ifdef _WIN32
    #include <windows.h>  // Windows-specific
#endif

class Utils {
public:
    static void OpenFileWithDefaultApp(const std::string &filepath) {
#ifdef _WIN32
        ShellExecuteA(NULL, "open", filepath.c_str(), NULL, NULL, SW_SHOWNORMAL);  // Windows
#elif __APPLE__
        std::string command = "open " + filepath;
        system(command.c_str()); // macOS
#elif __linux__
        std::string command = "xdg-open " + filepath;
        system(command.c_str());  // Linux
#endif
    }

    static void OpenFileInFileExplorer(const std::string &filepath) {
#ifdef _WIN32
        std::string command = "explorer /select,\"" + filepath + "\"";  // Select file in Explorer
        system(command.c_str());
#elif __APPLE__
        std::string command = "open -R " + filepath; // Reveal file in Finder
        system(command.c_str());
#elif __linux__
        std::string command = "xdg-open \"" + filepath.substr(0, filepath.find_last_of('/')) + "\"";  // Open folder
        system(command.c_str());
#endif
    }

    static bool EndsWith(const std::string &str, const std::string &suffix) {
        return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    static void CreateEmptyFile(const std::string &path) {
        std::ofstream file(path);
        file.close();
    }
};

#endif //UTILS_H
