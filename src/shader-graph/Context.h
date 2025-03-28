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
    Vector2 mousePos;
    MouseDragState &mouseDragState;
    Camera2D &camera;
    InteractionState interactionState;

    NodeInput *connectionInput;
    NodeOutput *connectionOutput;

    int selectedNodeUID = -1;

    std::list<Node> &nodes;

    Context(MouseDragState &mouseDragState, Camera2D &camera, std::list<Node> &nodes)
        : mousePos(), mouseDragState(mouseDragState), camera(camera), nodes(nodes) {
    }

    bool interactionStateLowerThan(InteractionState state) {
        return static_cast<int>(interactionState) < static_cast<int>(state);
    }
};

#endif //CONTEXT_H
