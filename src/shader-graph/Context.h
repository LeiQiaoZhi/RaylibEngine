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
    LineHovering, // for line
    Dragging, // for a node
    Connecting, // for connecting input/output
};

enum class ShowPreviewState {
    Individual,
    On,
    Off
};

struct Context {
    // states
    Vector2 mousePos;
    MouseDragState &mouseDragState;
    Camera2D &camera;
    InteractionState interactionState;

    // connection
    NodeInput *connectionInput;
    NodeOutput *connectionOutput;

    // selection
    uint selectedNodeUID = 0;
    NodeInput *selectedLine;

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
    ShowPreviewState showPreviewState = ShowPreviewState::Individual;

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

    void SelectNode(const int uid) {
        selectedNodeUID = uid;
        selectedLine = nullptr;
    }

    void SelectLine(NodeInput *line) {
        selectedLine = line;
        selectedNodeUID = 0;
    }

    void Update();
};

#endif //CONTEXT_H
