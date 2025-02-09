#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>  // For system()
#ifdef _WIN32
    #include <windows.h>  // Windows-specific
#endif

class Utils {
public:
    static void OpenFileWithDefaultApp(const std::string& filepath) {
#ifdef _WIN32
        ShellExecuteA(NULL, "open", filepath.c_str(), NULL, NULL, SW_SHOWNORMAL);  // Windows
#elif __APPLE__
        std::string command = "open " + filepath;
        system(command.c_str());  // macOS
#elif __linux__
        std::string command = "xdg-open " + filepath;
        system(command.c_str());  // Linux
#endif
    }
};

#endif //UTILS_H
