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

            if (j["name"].get<std::string>().find(' ') != std::string::npos) {
                std::cout << "Function name should not contain space: " << j["name"] << std::endl;
            }

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

        // final node function -- don't need for preview
        if (finalNode != nullptr) {
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
        }

        return oss.str();
    }

    inline std::string GetMain(const std::list<Node *> &orderedNodes, Node *previewFinalNode = nullptr) {
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

        // preview
        if (previewFinalNode != nullptr && previewFinalNode->outputs.size() > 0) {
            NodeOutput *output = &previewFinalNode->outputs[previewFinalNode->previewOutputIndex];
            std::string outputVar = output->GetVarName();
            if (output->type != ShaderType::Vec4) {
                std::string convertFunc = "convert_" + ShaderTypeToStringMap[output->type] + "_to_vec4";
                oss << "\t" << convertFunc << "(" << outputVar << ", finalColor);\n";
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
}

#endif //CODEGENERATION_H
