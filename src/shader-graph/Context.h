//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <list>

#include "../common/input/MouseDragState.h"

class Node;
class NodeInput;
class NodeOutput;

// Higher => more priority, override lower priority
enum class InteractionState {
    None = 0,
    Hovering, // for a node
    IOHovering, // for input/output circle
    Dragging, // for a node
    Connecting, // for connecting input/output
};

struct Context {
    // states
    Vector2 mousePos;
    MouseDragState &mouseDragState;
    Camera2D &camera;
    InteractionState interactionState;

    NodeInput *connectionInput;
    NodeOutput *connectionOutput;

    uint selectedNodeUID = 0;

    std::list<Node> &nodes;

    Node *FinalNode() const {
        return &nodes.front();
    }

    Node *FindNodeByUID(uint uid) const;

    // code
    std::string shaderCode;
    bool compileFlag = false;
    Node *nodeToDelete = nullptr;

    // settings
    bool showTypeInfo;

    Context(MouseDragState &mouseDragState, Camera2D &camera, std::list<Node> &nodes)
        : mousePos(), mouseDragState(mouseDragState), camera(camera), nodes(nodes) {
    }

    void SendShaderCode(const std::string &code) {
        shaderCode = code;
        compileFlag = true;
    }

    bool interactionStateLowerThan(InteractionState state) {
        return static_cast<int>(interactionState) < static_cast<int>(state);
    }

    void RemoveNode();

    void SaveGraph(const std::string &path);

    void LoadGraph(const std::string &path);
};

#endif //CONTEXT_H
