//
// Created by Qiaozhi Lei on 3/30/25.
//
#include "Context.h"

#include "CodeGeneration.h"
#include "nodes/Node.h"
#include "nodes/NodeGroup.h"


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
            if (selectedLine == &input) {
                selectedLine = nullptr;
            }
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
    j["showPreviewState"] = showPreviewState;

    j["cameraPos"] = {camera.target.x, camera.target.y};
    j["cameraZoom"] = camera.zoom;

    // nodes
    std::vector<nlohmann::json> nodesJson;
    for (auto &node: nodes) {
        nodesJson.emplace_back(node.ToJson());
    }
    j["nodes"] = nodesJson;

    // groups
    std::vector<nlohmann::json> groupsJson;
    for (auto &group: nodeGroups) {
        groupsJson.emplace_back(group.ToJson());
    }
    j["groups"] = groupsJson;

    JsonUtils::JsonToFile(j, path);
}

void Context::LoadGraph(const std::string &path) {
    const nlohmann::json j = JsonUtils::JsonFromFile(path);

    showTypeInfo = j.value("showTypeInfo", showTypeInfo);
    showPreviewState = j.value("showPreviewState", showPreviewState);

    // camera
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

    // groups
    nodeGroups.clear();
    if (j.contains("groups")) {
        for (const auto &groupJson: j["groups"]) {
            nodeGroups.emplace_back(groupJson);
        }
    } else {
        nodeGroups.emplace_back("Selection", true);
    }

    // load connections
    int i = 0;
    for (auto &node: nodes) {
        const auto &nodeJson = j["nodes"][i];
        node.LoadConnections(nodeJson, this);
        i++;
    }

    // recompile
    shaderCode = CodeGeneration::GenerateCode(FinalNode());
    compileFlag = true;
}

void Context::SelectNode(const int uid) {
    if (!IsKeyDown(KEY_LEFT_SHIFT))
        SelectionGroup()->Clear();
    SelectionGroup()->AddNode(uid);
    selectedLine = nullptr;
}

void Context::SelectLine(NodeInput *line) {
    selectedLine = line;
    SelectionGroup()->Clear();
}

NodeGroup *Context::SelectionGroup() const {
    return &nodeGroups.front();
}

void Context::Update() {
    if (nodeToDelete != nullptr) {
        RemoveNode();
    }
    if (groupToDelete != nullptr) {
        std::cout << "Deleting group: " << groupToDelete->name << std::endl;
        nodeGroups.remove_if([this](const NodeGroup &g) {
            return g.uid == groupToDelete->uid;
        });
        groupToDelete = nullptr;
    }

    mouseDragState.Update();

    if (interactionState == InteractionState::Dragging) {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    } else if (interactionState == InteractionState::Connecting || interactionState == InteractionState::Hovering) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsKeyPressed(KEY_DELETE)) {
        std::cout << "Delete key pressed" << std::endl;

        for (auto nodeUID: SelectionGroup()->nodeUIDs) {
            Node *node = FindNodeByUID(nodeUID);
            if (node != nullptr) {
                std::cout << "Deleting node: " << node->name << std::endl;
                nodeToDelete = node;
                RemoveNode();
            }
        }

        if (selectedLine != nullptr) {
            selectedLine->source->targets.remove(selectedLine);
            selectedLine->source = nullptr;
            selectedLine = nullptr;
        }
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SUPER)) {
        if (IsKeyPressed(KEY_V)) {
            std::cout << "Ctrl + V pressed" << std::endl;
            // past selected node
            for (auto nodeUID: SelectionGroup()->nodeUIDs) {
                Node *node = FindNodeByUID(nodeUID);
                if (node != nullptr) {
                    std::cout << "Pasting node: " << node->name << std::endl;
                    nodes.emplace_back(node);
                    nodes.back().position = mousePos + node->position - SelectionGroup()->position;
                }
            }
        }

        if (IsKeyPressed(KEY_A)) {
            std::cout << "Ctrl + A pressed" << std::endl;
            for (auto &node: nodes) {
                SelectionGroup()->AddNode(&node);
            }
        }

        if (IsKeyPressed(KEY_G)) {
            std::cout << "Ctrl + G pressed" << std::endl;
            if (!SelectionGroup()->Empty()) {
                nodeGroups.emplace_back(SelectionGroup(), "");
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        SelectionGroup()->Clear();
    }
}
