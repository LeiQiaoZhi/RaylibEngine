#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class JsonUtils {
public:
    static void JsonToFile(const nlohmann::json &_json, const std::string &_path) {
        std::ofstream out_file(_path.c_str(), std::ofstream::out | std::ofstream::trunc);

        if (!out_file.is_open()) {
            std::cerr << "Failed to open " << _path << '\n';
            return;
        }

        out_file << _json.dump(4);
        out_file.close();

        std::cout << "JSON saved to " << _path << '\n';
    }

    static nlohmann::json JsonFromFile(const std::string &_path) {
        std::ifstream in_file(_path.c_str());

        if (!in_file.is_open()) {
            std::cerr << "Failed to open " << _path << '\n';
            return nlohmann::json();
        }

        nlohmann::json j = nlohmann::json::parse(in_file);
        in_file.close();

        std::cout << "JSON loaded from " << _path << '\n';

        return j;
    }
};

#endif //JSONUTILS_H
