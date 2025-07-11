//
// Created by Qiaozhi Lei on 3/28/25.
//

#ifndef CODEGENERATION_H
#define CODEGENERATION_H

#include "raylib.h"
#include <string>
#include "utils/Utils.h"
#include "utils/JsonUtils.h"
#include "nodes/Node.h"
#include "nodes/NodeInput.h"
#include "nodes/NodeOutput.h"

namespace CodeGeneration {
    enum class ShaderFileType {
        GLSL,
        HLSL
    };

    inline std::vector<std::tuple<bool, ShaderType, std::string> > hlslIOs = {
        // true -- in, false -- inout
        {true, ShaderType::Vec2, "fragTexCoord"},
        {true, ShaderType::Texture2D, "mainTex"},
        {true, ShaderType::Sampler2D, "mainTexSampler"},
        {false, ShaderType::Vec4, "finalColor"}
    };

    inline std::string ShaderTypeToString(ShaderType type, ShaderFileType exportType = ShaderFileType::GLSL) {
        if (exportType == ShaderFileType::GLSL) {
            return ShaderTypeToStringMap[type];
        } else {
            return ShaderTypeToStringMapHLSL[type];
        }
    }

    inline std::string ShaderTypeFromJson(const nlohmann::json &j, ShaderFileType exportType = ShaderFileType::GLSL) {
        std::string typeStr = j["type"].get<std::string>();
        ShaderType type = ShaderTypeMap[typeStr];
        if (exportType == ShaderFileType::GLSL) {
            return ShaderTypeToStringMap[type];
        } else {
            return ShaderTypeToStringMapHLSL[type];
        }
    }

    inline std::set<Node *> FilterNodes(Node *finalNode) {
        std::set<Node *> filteredNodes;
        // DFS to find all nodes reachable from final node
        std::list<Node *> toExplore = {finalNode};
        while (!toExplore.empty()) {
            Node *node = toExplore.front();
            toExplore.pop_front();
            filteredNodes.insert(node);

            std::set<Node *> neighbours = node->GetNeighboursFromInputs();
            toExplore.insert(toExplore.end(), neighbours.begin(), neighbours.end());
        }
        return filteredNodes;
    }

    inline void TopologicalSortVisit(Node *node, std::set<Node *> &visited, std::list<Node *> &orderedNodes,
                                     const std::set<Node *> &nodes) {
        visited.insert(node);
        const std::set<Node *> neighbours = node->GetNeighboursFromOutputs();
        for (auto &neighbour: neighbours) {
            if (visited.find(neighbour) == visited.end() && nodes.find(neighbour) != nodes.end()) {
                TopologicalSortVisit(neighbour, visited, orderedNodes, nodes);
            }
        }
        orderedNodes.push_front(node);
    }

    inline std::list<Node *> TopologicalSort(const std::set<Node *> &nodes) {
        std::set<Node *> visited;
        std::list<Node *> orderedNodes;
        for (auto &node: nodes) {
            if (visited.find(node) == visited.end()) {
                TopologicalSortVisit(node, visited, orderedNodes, nodes);
            }
        }
        return orderedNodes;
    }


    inline std::string GetPrefix(ShaderFileType exportType = ShaderFileType::GLSL) {
        std::ostringstream oss;
        if (exportType == ShaderFileType::GLSL) {
            oss << "#version 330\n";
            oss << "in vec2 fragTexCoord;\n";
            oss << "uniform sampler2D mainTex;\n";
            oss << "out vec4 finalColor;\n";
        }

        return oss.str();
    }

    inline std::string GetFunctions(Node *finalNode, ShaderFileType fileType = ShaderFileType::GLSL) {
        std::vector<std::tuple<bool, ShaderType, std::string> > mainIOs =
                (fileType == ShaderFileType::GLSL)
                    ? std::vector<std::tuple<bool, ShaderType, std::string> >{}
                    : hlslIOs;
        std::ostringstream oss;

        char *convert =
                fileType == ShaderFileType::GLSL
                    ? LoadFileText(INTERNAL_ASSET_DIR "/shaders/convert.glsl")
                    : LoadFileText(INTERNAL_ASSET_DIR "/shaders/convert.hlsl");
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

            if (j["name"].get<std::string>().find(' ') != std::string::npos) {
                std::cout << "Function name should not contain space: " << j["name"] << std::endl;
            }

            // signature
            oss << "void " << j["name"].get<std::string>() << "(";
            for (auto &mainIO: mainIOs) {
                oss << (std::get<0>(mainIO) ? "in" : "inout") << " "
                        << ShaderTypeToString(std::get<1>(mainIO), fileType)
                        << " " << std::get<2>(mainIO) << ", ";
            }
            for (auto &input: j["inputs"]) {
                oss << "in " << ShaderTypeFromJson(input, fileType) << " " << input["name"].get<std::string>() << ", ";
            }
            for (int i = 0; i < j["outputs"].size(); i++) {
                oss << "out " << ShaderTypeFromJson(j["outputs"][i], fileType) << " "
                        << j["outputs"][i]["name"].get<std::string>();
                if (i < j["outputs"].size() - 1)
                    oss << ", ";
            }
            oss << ") {\n";

            // body
            oss << (fileType == ShaderFileType::GLSL ? j["glsl"].get<std::string>() : j["hlsl"].get<std::string>())
                    << "\n}\n\n";
        }

        // final node function -- don't need for preview
        if (finalNode != nullptr) {
            std::vector<std::string> ioVars;
            ioVars.reserve(finalNode->inputs.size() + finalNode->outputs.size());
            oss << "void " << finalNode->name << "(";
            for (auto &mainIO: mainIOs) {
                oss << (std::get<0>(mainIO) ? "in" : "inout") << " "
                        << ShaderTypeToString(std::get<1>(mainIO), fileType)
                        << " " << std::get<2>(mainIO) << ", ";
            }
            for (auto &input: finalNode->inputs) {
                ioVars.emplace_back("in " + ShaderTypeToString(input.type, fileType) + " " + input.name);
            }
            for (auto &output: finalNode->outputs) {
                ioVars.emplace_back("out " + ShaderTypeToString(output.type, fileType) + " " + output.name);
            }
            for (int i = 0; i < ioVars.size(); i++) {
                oss << ioVars[i];
                if (i < ioVars.size() - 1)
                    oss << ", ";
            }
            oss << ") {\n";
            oss <<
                    (fileType == ShaderFileType::GLSL ? finalNode->glsl : finalNode->hlsl)
                    << "\n}\n\n";
        }

        return oss.str();
    }

    inline std::string GetMain(const std::list<Node *> &orderedNodes, Node *previewFinalNode = nullptr,
                               ShaderFileType fileType = ShaderFileType::GLSL) {
        std::vector<std::tuple<bool, ShaderType, std::string> > mainIOs =
                (fileType == ShaderFileType::GLSL)
                    ? std::vector<std::tuple<bool, ShaderType, std::string> >{}
                    : hlslIOs;

        std::ostringstream oss;
        oss << "void main(";
        for (int i = 0; i < mainIOs.size(); i++) {
            oss << (std::get<0>(mainIOs[i]) ? "in" : "inout") << " "
                    << ShaderTypeToString(std::get<1>(mainIOs[i]), fileType)
                    << " " << std::get<2>(mainIOs[i]);
            if (i < mainIOs.size() - 1)
                oss << ", ";
        }
        oss << "){\n";
        if (previewFinalNode != nullptr && fileType == ShaderFileType::GLSL) {
            oss << "mainTex;\n"; // prevent unused variable removal
        }

        for (auto *node: orderedNodes) {
            // declare variables
            for (auto &input: node->inputs) {
                std::string inputVar = input.GetVarName();
                oss << "\t" << ShaderTypeToString(input.type, fileType) << " " << inputVar << ";\n";
                // give values to inputs
                if (input.source != nullptr) {
                    std::string sourceVar = input.source->GetVarName();
                    if (input.source->type != input.type) {
                        std::string convertFunc =
                                "convert_" + ShaderTypeToString(input.source->type, fileType) + "_to_" +
                                ShaderTypeToString(input.type, fileType);
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
                oss << "\t" << ShaderTypeToString(output.type, fileType) << " " << outputVar << ";\n";
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
            for (auto &mainIO: mainIOs) {
                oss << std::get<2>(mainIO) << ", ";
            }
            for (int i = 0; i < ioVars.size(); i++) {
                oss << ioVars[i];
                if (i < ioVars.size() - 1)
                    oss << ", ";
            }
            oss << ");\n";
        }

        // preview
        if (previewFinalNode != nullptr && previewFinalNode->outputs.size() > 0) {
            NodeOutput *output = &previewFinalNode->outputs[previewFinalNode->previewOutputIndex];
            std::string outputVar = output->GetVarName();
            if (output->type != ShaderType::Vec4) {
                std::string convertFunc = "convert_" + ShaderTypeToString(output->type, fileType)
                                          + "_to_" + ShaderTypeToString(ShaderType::Vec4, fileType);
                oss << "\t" << convertFunc << "(" << outputVar << ", finalColor);\n";
            } else {
                oss << "\tfinalColor = " << outputVar << ";\n";
            }
            oss << "\tfinalColor.a = 1.0;\n";
        }

        oss << "}\n";

        return oss.str();
    }

    inline std::string GeneratePreviewCode(Node *finalNode) {
        std::set<Node *> filteredNodes = FilterNodes(finalNode);
        std::list<Node *> orderedNodes = TopologicalSort(filteredNodes);
        const std::string prefix = GetPrefix();
        const std::string functions = GetFunctions(nullptr);
        const std::string main = GetMain(orderedNodes, finalNode);
        return prefix + functions + main;
    }

    inline std::string GenerateCode(Node *finalNode, ShaderFileType fileType = ShaderFileType::GLSL) {
        std::set<Node *> filteredNodes = FilterNodes(finalNode);
        std::list<Node *> orderedNodes = TopologicalSort(filteredNodes);
        const std::string prefix = GetPrefix(fileType);
        const std::string functions = GetFunctions(finalNode, fileType);
        const std::string main = GetMain(orderedNodes, nullptr, fileType);
        return prefix + functions + main;
    }
}

#endif //CODEGENERATION_H
