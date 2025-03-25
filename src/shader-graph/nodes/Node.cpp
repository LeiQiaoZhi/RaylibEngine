//
// Created by Qiaozhi Lei on 3/24/25.
//

#include "Node.h"

#include <iostream>

#include "raylib.h"

void Node::OnDraw() {
    DrawRectangle(position.x, position.y, size.x, size.y, BLACK);
}

void Node::Update(Context context) {
    const MouseDragState dragState = context.mouseDragState;
    const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
    const bool isHovered =
            CheckCollisionPointRec(startMouseWorldPos, Rectangle{position.x, position.y, size.x, size.y});
    if (dragState.JustFinishedDragging() && isHovered) {
        position = context.mousePos;
    }
}
