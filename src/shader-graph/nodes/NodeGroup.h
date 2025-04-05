//
// Created by Qiaozhi Lei on 4/3/25.
//

#ifndef NODEGROUP_H
#define NODEGROUP_H

#include "Node.h"

class NodeGroup : public Node {
public:
    explicit NodeGroup(std::string name, bool isSelectionGroup) : Node(), isSelectionGroup(isSelectionGroup) {
        this->name = std::move(name);
        uid = Utils::GenerateUID(this->name);
    }

    explicit NodeGroup(NodeGroup* copy, std::string name) : Node() {
        this->name = std::move(name);
        for (auto &uid: copy->nodeUIDs) {
            this->nodeUIDs.push_back(uid);
        }
        this->uid = Utils::GenerateUID(this->name);
        isSelectionGroup = false;
        nameEditMode = true;
    }

    explicit NodeGroup(nlohmann::json j) : Node() {
        name = j["name"];
        uid = j["uid"];
        isSelectionGroup = j["isSelectionGroup"];
        for (auto &uid: j["nodeUIDs"]) {
            nodeUIDs.push_back(uid);
        }
    }

    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["name"] = name.c_str();
        j["uid"] = uid;
        j["isSelectionGroup"] = isSelectionGroup;
        j["nodeUIDs"] = nodeUIDs;
        return j;
    }

    void OnDraw(Context &context) override;

    void Update(Context &context) override;

    void Resolve(Context &context) override;

    void OnEditorGUI(Rectangle &rect, Context &context) override;

    void AddNode(Node *node) {
        nodeUIDs.push_back(node->uid);
    }

    void AddNode(int uid) {
        nodeUIDs.push_back(uid);
    }

    void Clear() {
        nodeUIDs.clear();
    }

    bool Empty() const {
        return nodeUIDs.empty();
    }

    bool Contains(Node *node) const {
        for (auto &uid: nodeUIDs) {
            if (node->uid == uid) {
                return true;
            }
        }
        return false;
    }

    std::list<int> nodeUIDs;
    bool isSelectionGroup;
    bool nameEditMode;
};


#endif //NODEGROUP_H
