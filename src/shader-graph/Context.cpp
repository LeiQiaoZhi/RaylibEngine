//
// Created by Qiaozhi Lei on 3/30/25.
//
#include "Context.h"
#include "nodes/Node.h"


void Context::RemoveNode() {
    for (auto &input : nodeToDelete->inputs) {
        if (input.source != nullptr) {
            input.source->targets.remove(&input);
        }
    }

    for (auto &output : nodeToDelete->outputs) {
        for (auto &target : output.targets) {
            target->source = nullptr;
        }
    }

    // remove node, compare using uid
    nodes.remove_if([this](const Node &n) {
        return n.uid == nodeToDelete->uid;
    });

    nodeToDelete = nullptr;
}