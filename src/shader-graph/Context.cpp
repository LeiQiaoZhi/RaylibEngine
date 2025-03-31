//
// Created by Qiaozhi Lei on 3/30/25.
//
#include "Context.h"
#include "nodes/Node.h"


Node *Context::FindNodeByUID(uint uid) const {
    for (auto &node: nodes) {
        if (node.uid == uid) {
            return &node;
        }
    }
    return nullptr;
}

void Context::RemoveNode() {
    for (auto &input: nodeToDelete->inputs) {
        if (input.source != nullptr) {
            input.source->targets.remove(&input);
        }
    }

    for (auto &output: nodeToDelete->outputs) {
        for (auto &target: output.targets) {
            target->source = nullptr;
        }
    }

    // remove node, compare using uid
    nodes.remove_if([this](const Node &n) {
        return n.uid == nodeToDelete->uid;
    });

    nodeToDelete = nullptr;
}

void Context::SaveGraph(const std::string &path) {
    nlohmann::json j;

    j["showTypeInfo"] = showTypeInfo;
    j["time"] = time(nullptr);
    j["selectedNodeUID"] = selectedNodeUID;
    j["cameraPos"] = {camera.target.x, camera.target.y};
    j["cameraZoom"] = camera.zoom;

    // nodes
    std::vector<nlohmann::json> nodesJson;
    for (auto &node: nodes) {
        nodesJson.emplace_back(node.ToJson());
    }
    j["nodes"] = nodesJson;

    JsonUtils::JsonToFile(j, path);
}

void Context::LoadGraph(const std::string &path) {
    const nlohmann::json j = JsonUtils::JsonFromFile(path);

    showTypeInfo = j.value("showTypeInfo", showTypeInfo);
    selectedNodeUID = j.value("selectedNodeUID", selectedNodeUID);
    if (j.contains("cameraPos")) {
        camera.target.x = j["cameraPos"][0];
        camera.target.y = j["cameraPos"][1];
    }
    camera.zoom = j.value("cameraZoom", camera.zoom);

    // nodes
    nodes.clear();
    for (const auto &nodeJson: j["nodes"]) {
        nodes.emplace_back(nodeJson, true);
    }

    // load connections
    int i = 0;
    for (auto &node: nodes) {
        const auto &nodeJson = j["nodes"][i];
        node.LoadConnections(nodeJson, this);
        i++;
    }

    // recompile
    compileFlag = true;
}
