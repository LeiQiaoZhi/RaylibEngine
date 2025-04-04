//
// Created by Qiaozhi Lei on 4/3/25.
//

#ifndef NODEGROUP_H
#define NODEGROUP_H

#include "Node.h"

class NodeGroup : public Node {
public:
    explicit NodeGroup(std::string name) : Node() {
        this->name = std::move(name);
        uid = Utils::GenerateUID(this->name);
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

    bool Contains(Node *node) const {
        for (auto &uid: nodeUIDs) {
            if (node->uid == uid) {
                return true;
            }
        }
        return false;
    }

    std::list<int> nodeUIDs;
};


#endif //NODEGROUP_H
