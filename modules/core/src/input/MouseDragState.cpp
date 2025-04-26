#include "MouseDragState.h"
#include "../GCM/Components/Component.h"

#include "../GCM/GameObject.h"

void MouseDragState::Update() {
    previousIsDragging = isDragging;
    if (isDragging) {
        // end drag
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
            endDragPosition = GetMousePosition();
        }
        else {
            delta = GetMousePosition() - previousDragPosition;
            previousDragPosition = GetMousePosition();
        }
    } else {
        // start drag
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isDragging = true;
            startDragPosition = GetMousePosition();
            previousDragPosition = startDragPosition;
        }
    }
}

void MouseDragState::OnDrawGizmos(const Component *component) const {
    if (!isDragging) return;
    const Scene *scene = component->gameObject->scene;
    const Vector2 start = scene->GlobalToLocalScreenSpace(startDragPosition);
    const Vector2 current = scene->GlobalToLocalScreenSpace(GetMousePosition());
    DrawLineV(start, current, YELLOW);
}
