//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include "../common/input/MouseDragState.h"

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

    Context(MouseDragState &mouseDragState, Camera2D &camera)
        : mousePos(), mouseDragState(mouseDragState), camera(camera) {
    }

    bool interactionStateLowerThan(InteractionState state) {
        return static_cast<int>(interactionState) < static_cast<int>(state);
    }
};

#endif //CONTEXT_H
