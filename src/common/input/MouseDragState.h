#ifndef MOUSEDRAGSTATE_H
#define MOUSEDRAGSTATE_H

#include "raylib.h"

class Component;

class MouseDragState {
public:
    void Update();
    void OnDrawGizmos(const Component* component) const;

    bool JustFinishedDragging() const {
        return previousIsDragging && !isDragging;
    }

    bool JustStartedDragging() const {
        return isDragging && !previousIsDragging;
    }

public:
    bool isDragging = false;
    Vector2 startDragPosition = {0, 0};
    Vector2 endDragPosition = {0, 0};
    Vector2 delta = {0, 0};
    Vector2 previousDragPosition;

private:
    bool previousIsDragging = false;
};

#endif //MOUSEDRAGSTATE_H
