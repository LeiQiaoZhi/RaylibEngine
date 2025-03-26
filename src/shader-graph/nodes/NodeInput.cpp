//
// Created by Qiaozhi Lei on 3/24/25.
//

#include "NodeInput.h"
#include "NodeOutput.h"

#include "../../common/editor/Editor.h"
#include "../../common/utils/RaylibUtils.h"

Color NodeInput::GetColor() const {
    const float alpha = hovering ? 1.0f : 0.7f;
    if (source == nullptr) {
        return Fade(GRAY, alpha);
    }
    return Fade(GREEN, alpha);
}

void NodeInput::Draw(Rectangle &rect) {
    // circle
    circleCenter = (Vector2){rect.x + rect.height / 2, rect.y + rect.height / 2};
    radius = rect.height / 2;
    DrawCircle(rect.x + rect.height / 2 + Editor::SmallGap(), rect.y + rect.height / 2,
               rect.height / 2, GetColor());

    // label
    const float textWidth = MeasureTextEx(Editor::GetFont(), name.c_str(), Editor::TextSizeF(), 1).x;
    rect.width = std::max(rect.width, textWidth + Editor::SmallGap() * 3 + rect.height);
    DrawTextEx(Editor::GetFont(), name.c_str(),
               {rect.x + rect.height + Editor::SmallGap() * 2, rect.y},
               Editor::TextSizeF(), 1, WHITE);

    rect.y += rect.height + Editor::MediumGap();

    // line
    if (source != nullptr) {
        const float handleLength = abs(std::max((circleCenter - source->circleCenter).x,
                                                (circleCenter - source->circleCenter).y)) / 2;
        RaylibUtils::DrawLineBezier(
            circleCenter, circleCenter + Vector2{-handleLength, 0},
            source->circleCenter + Vector2{handleLength, 0}, source->circleCenter,
            2, Fade(GREEN, 0.5f));
    }
}

void NodeInput::Update(Context &context) {
    const MouseDragState dragState = context.mouseDragState;
    const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
    const bool wasHovered = CheckCollisionPointCircle(startMouseWorldPos, circleCenter, radius);

    // handle hovering
    const bool isHovering = CheckCollisionPointCircle(context.mousePos, circleCenter, radius * 2.);
    if ((wasHovered || isHovering) && context.interactionStateLowerThan(InteractionState::IOHovering)) {
        context.interactionState = InteractionState::IOHovering;
        hovering = true;
    } else {
        hovering = false;
    }

    // handle dragging
    // if (dragState.JustFinishedDragging() && wasHovered
    //     && context.interactionStateLowerThan(InteractionState::Connecting)) {
    //     context.interactionState = InteractionState::Connecting;
    // }
}

void NodeInput::Resolve(Context &context) {
    const bool isHovering = CheckCollisionPointCircle(context.mousePos, circleCenter, radius * 2.);

    // resolve being connected
    if (context.mouseDragState.JustFinishedDragging() && context.interactionState == InteractionState::Connecting &&
        isHovering) {
        if (context.connectionOutput != nullptr) {
            context.connectionOutput->target = this;
            source = context.connectionOutput;
        }
    }
}
