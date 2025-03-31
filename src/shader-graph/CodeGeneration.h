//
// Created by Qiaozhi Lei on 3/28/25.
//

#ifndef CODEGENERATION_H
#define CODEGENERATION_H

#include "raylib.h"
#include <string>
#include "../common/utils/Utils.h"
#include "../common/utils/JsonUtils.h"
#include "nodes/Node.h"
#include "nodes/NodeInput.h"
#include "nodes/NodeOutput.h"

namespace CodeGeneration {
    inline std::vector<std::string> GetOptions() {
        std::vector<std::string> options;
        const std::string path = INTERNAL_ASSET_DIR "/nodes";
        const FilePathList files = LoadDirectoryFilesEx(path.c_str(), NULL, false);
        for (int i = 0; i < files.count; i++) {
            const char *completeFilePath = files.paths[i];
            const char *file = GetFileName(completeFilePath);
            if (file[0] == '.' || !Utils::EndsWith(file, ".json"))
                continue;

            options.push_back(file);
        }
        UnloadDirectoryFiles(files);
        return options;
    }

    inline std::string GetPrefix() {
        std::ostringstream oss;
        oss << "#version 330\n";
        oss << "in vec2 fragTexCoord;\n";
        oss << "out vec4 finalColor;\n";

        return oss.str();
    }

    inline std::string GetFunctions(Node *finalNode) {
        std::ostringstream oss;

        char *convert = LoadFileText(INTERNAL_ASSET_DIR "/shaders/convert.glsl");
        oss << convert << "\n";
        UnloadFileText(convert);

        const std::string path = INTERNAL_ASSET_DIR "/nodes";
        const FilePathList files = LoadDirectoryFilesEx(path.c_str(), NULL, false);
        for (int i = 0; i < files.count; i++) {
            const char *completeFilePath = files.paths[i];
            const char *file = GetFileName(completeFilePath);
            if (file[0] == '.' || !Utils::EndsWith(file, ".json"))
                continue;

            nlohmann::json j = JsonUtils::JsonFromFile(completeFilePath);

            // signature
            oss << "void " << j["name"].get<std::string>() << "(";
            for (auto &input: j["inputs"]) {
                oss << "in " << input["type"].get<std::string>() << " " << input["name"].get<std::string>() << ", ";
            }
            for (int i = 0; i < j["outputs"].size(); i++) {
                oss << "out " << j["outputs"][i]["type"].get<std::string>() << " " << j["outputs"][i]["name"].get<
                    std::string>();
                if (i < j["outputs"].size() - 1)
                    oss << ", ";
            }
            oss << ") {\n";

            // body
            oss << j["glsl"].get<std::string>() << "\n}\n\n";
        }

        // final node
        std::vector<std::string> ioVars;
        ioVars.reserve(finalNode->inputs.size() + finalNode->outputs.size());
        oss << "void " << finalNode->name << "(";
        for (auto &input: finalNode->inputs) {
            ioVars.emplace_back("in " + ShaderTypeToStringMap[input.type] + " " + input.name);
        }
        for (auto &output: finalNode->outputs) {
            ioVars.emplace_back("out " + ShaderTypeToStringMap[output.type] + " " + output.name);
        }
        for (int i = 0; i < ioVars.size(); i++) {
            oss << ioVars[i];
            if (i < ioVars.size() - 1)
                oss << ", ";
        }
        oss << ") {\n";
        oss << finalNode->glsl << "\n}\n\n";

        return oss.str();
    }

    inline std::string GetMain(const std::list<Node *> &orderedNodes) {
        std::ostringstream oss;
        oss << "void main() {\n";

        for (auto *node: orderedNodes) {
            // declare variables
            for (auto &input: node->inputs) {
                std::string inputVar = input.GetVarName();
                oss << "\t" << ShaderTypeToStringMap[input.type] << " " << inputVar << ";\n";
                // give values to inputs
                if (input.source != nullptr) {
                    std::string sourceVar = input.source->GetVarName();
                    if (input.source->type != input.type) {
                        std::string convertFunc = "convert_" + ShaderTypeToStringMap[input.source->type] + "_to_" +
                                                  ShaderTypeToStringMap[input.type];
                        oss << "\t" << convertFunc << "(" << sourceVar << "," << inputVar << ");\n";
                    } else {
                        oss << "\t" << inputVar << " = " << sourceVar << ";\n";
                    }
                } else if (input.type == ShaderType::Float) {
                    oss.precision(10);
                    oss << "\t" << inputVar << " = " << input.floatValue << ";\n";
                }
            }
            for (auto &output: node->outputs) {
                std::string outputVar = "output_" + std::to_string(node->uid) + "_" + output.name;
                oss << "\t" << ShaderTypeToStringMap[output.type] << " " << outputVar << ";\n";
            }

            // call function
            oss << "\t" << node->name << "(";
            std::vector<std::string> ioVars;
            for (auto &input: node->inputs) {
                ioVars.emplace_back(input.GetVarName());
            }
            for (auto &output: node->outputs) {
                ioVars.emplace_back(output.GetVarName());
            }
            for (int i = 0; i < ioVars.size(); i++) {
                oss << ioVars[i];
                if (i < ioVars.size() - 1)
                    oss << ", ";
            }
            oss << ");\n";
        }

        oss << "}\n";
        return oss.str();
    }
}

#endif //CODEGENERATION_H
