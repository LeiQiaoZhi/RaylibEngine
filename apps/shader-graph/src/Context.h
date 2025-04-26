//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <list>
#include <map>

#include "input/MouseDragState.h"

class NodeGroup;
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
    Camera, // camera interactions
};

inline std::map<InteractionState, std::string> InteractionStateToStringMap = {
    {InteractionState::None, "None"},
    {InteractionState::Hovering, "Hovering"},
    {InteractionState::IOHovering, "IOHovering"},
    {InteractionState::LineHovering, "LineHovering"},
    {InteractionState::Dragging, "Dragging"},
    {InteractionState::Connecting, "Connecting"},
    {InteractionState::Camera, "Camera"},
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
    NodeInput *selectedLine;

    std::list<Node> &nodes;
    std::list<NodeGroup> &nodeGroups;

    Node *FinalNode() const {
        return &nodes.front();
    }

    Node *FindNodeByUID(uint uid) const;

    // code
    std::string shaderCode;
    bool compileFlag = false;
    Texture2D mainTexture;

    // delete
    Node *nodeToDelete = nullptr;
    NodeGroup *groupToDelete;

    // settings
    bool showTypeInfo;
    ShowPreviewState showPreviewState = ShowPreviewState::Individual;

    Context(MouseDragState &mouseDragState, Camera2D &camera, std::list<Node> &nodes, std::list<NodeGroup> &nodeGroups)
        : mousePos(), mouseDragState(mouseDragState), camera(camera), nodes(nodes), nodeGroups(nodeGroups) {
    }

    void SendShaderCode(const std::string &code) {
        shaderCode = code;
        compileFlag = true;
    }

    bool interactionStateLowerThan(InteractionState state) const {
        return static_cast<int>(interactionState) < static_cast<int>(state);
    }

    void RemoveNode();

    void SaveGraph(const std::string &path);

    void LoadGraph(const std::string &path);

    void SelectNode(const int uid);

    void SelectLine(NodeInput *line);

    NodeGroup *SelectionGroup() const;

    void Update();
};

#endif //CONTEXT_H
