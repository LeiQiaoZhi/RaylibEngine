//
// Created by Qiaozhi Lei on 3/24/25.
//

#include "NodeOutput.h"

#include "Node.h"
#include "../../common/editor/Editor.h"

Color NodeOutput::GetColor() const {
    const float alpha = hovering ? 1.0f : 0.7f;
    if (target == nullptr) {
        return Fade(GRAY, alpha);
    }
    return Fade(GREEN, alpha);
}

void NodeOutput::Draw(Rectangle &rect) {
    const float endX = rect.x + rect.width - rect.height / 2 - Editor::SmallGap();
    circleCenter = (Vector2){endX, rect.y + rect.height / 2};
    radius = rect.height / 2;

    DrawCircle(endX, rect.y + rect.height / 2,
               rect.height / 2, GetColor());

    const float textWidth = MeasureTextEx(Editor::GetFont(), name.c_str(), Editor::TextSizeF(), 1).x;
    DrawTextEx(Editor::GetFont(), name.c_str(),
               {endX - rect.height / 2 - Editor::SmallGap() - textWidth, rect.y},
               Editor::TextSizeF(), 1, WHITE);

    // type
    DrawTextEx(Editor::GetFont(), ShaderTypeToStringMap[type].c_str(),
               {endX + rect.height / 2 + Editor::SmallGap(), rect.y},
               Editor::TextSizeF(), 1, GRAY);


    rect.y += rect.height + Editor::MediumGap();
}

float NodeOutput::GetWidth() const {
    const float textWidth = MeasureTextEx(Editor::GetFont(), name.c_str(), Editor::TextSizeF(), 1).x;
    return textWidth + Editor::SmallGap() * 2 + Editor::TextSizeF();
}

void NodeOutput::Update(Context &context) {
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
    if (dragState.JustStartedDragging() && isHovering) {
        dragging = true;
    }

    if (dragging && context.interactionStateLowerThan(InteractionState::Connecting)) {
        context.interactionState = InteractionState::Connecting;
        context.connectionOutput = this;
    }

    if (dragState.JustFinishedDragging()) {
        dragging = false;
    }
}

void NodeOutput::Resolve(Context &context) {
}

std::string NodeOutput::GetVarName() const {
    return "output_" + std::to_string(parentNode->uid) + "_" + name;
}
