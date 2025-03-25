//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include "../common/input/MouseDragState.h"

struct Context {
    Vector2 mousePos;
    MouseDragState &mouseDragState;
    Camera2D &camera;

    Context (MouseDragState &mouseDragState, Camera2D &camera)
        : mousePos(), mouseDragState(mouseDragState), camera(camera) {
    }
};

#endif //CONTEXT_H
