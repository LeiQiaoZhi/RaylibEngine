//
// Created by Qiaozhi Lei on 3/24/25.
//

#include <iostream>

#include "Node.h"
#include "raylib.h"
#include "raymath.h"
#include "../../common/editor/Editor.h"

void Node::OnDraw() {
    // background
    if (hovering) {
        static float edge = 4;
        DrawRectangle(position.x - edge / 2, position.y - edge / 2, size.x + edge, size.y + edge, GRAY);
    }
    DrawRectangle(position.x, position.y, size.x, size.y, Fade(BLACK, hovering ? 1.0f : 0.7f));

    // title
    const std::string title = "Node";
    const float textWidth = MeasureTextEx(Editor::GetFont(), title.c_str(), Editor::TextSizeF(), 1).x;
    Rectangle titleRect = {
        position.x + size.x / 2 - textWidth / 2, position.y + Editor::SmallGap(), 0, Editor::TextSizeF()
    };
    DrawTextEx(Editor::GetFont(), title.c_str(), {titleRect.x, titleRect.y}, Editor::TextSizeF(), 2, WHITE);
    titleRect.y += Editor::TextSizeF() + Editor::SmallGap();
    DrawLineEx({position.x + Editor::SmallGap(), titleRect.y},
               {position.x + size.x - Editor::SmallGap(), titleRect.y}, 1, Fade(WHITE, 0.3f));
    titleRect.y += Editor::MediumGap();

    // inputs
    Rectangle inputRect = {position.x, titleRect.y, 0, Editor::TextSizeF()};
    for (auto &input: inputs) {
        input.Draw(inputRect);
    }
    size.x = inputRect.width;

    // outputs
    float maxWidth = 0;
    for (auto &output: outputs) {
        maxWidth = std::max(maxWidth, output.GetWidth());
    }
    size.x += maxWidth + Editor::LargeGap();

    Rectangle outputRect = {position.x + size.x, titleRect.y, 0, Editor::TextSizeF()};
    for (auto &output: outputs) {
        output.Draw(outputRect);
    }

    // preview

    size.y = inputRect.y - position.y;
}


void Node::Update(Context &context) {
    const MouseDragState dragState = context.mouseDragState;
    const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
    const bool wasHovered =
            CheckCollisionPointRec(startMouseWorldPos, Rectangle{position.x, position.y, size.x, size.y});

    // handle connecting
    for (auto &input: inputs) {
        input.Update(context);
    }
    for (auto &output: outputs) {
        output.Update(context);
    }

    // handle hovering
    const bool isHovering = CheckCollisionPointRec(context.mousePos, Rectangle{
                                                       position.x, position.y, size.x, size.y
                                                   });
    if ((wasHovered || isHovering) && context.interactionStateLowerThan(InteractionState::Hovering)) {
        context.interactionState = InteractionState::Hovering;
        hovering = true;
    } else {
        hovering = false;
    }

    // handle dragging
    if (dragState.JustStartedDragging() && isHovering && context.
        interactionStateLowerThan(InteractionState::Dragging)) {
        dragging = true;
    }
    if (dragging && context.interactionStateLowerThan(InteractionState::Dragging)) {
        context.interactionState = InteractionState::Dragging;
    }
    if (dragState.JustFinishedDragging()) {
        dragging = false;
    }
}

void Node::Resolve(Context &context) {
    // resolve connections
    for (auto &input: inputs) {
        input.Resolve(context);
    }
    for (auto &output: outputs) {
        output.Resolve(context);
    }

    // resolve dragging
    if (dragging && context.interactionState == InteractionState::Dragging) {
        position += context.mouseDragState.delta;
    }
}

void Node::OnEditorGUI(Rectangle &rect) {
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, TextFormat("UID: %i", uid));
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // inputs
    inputFoldout.OnEditorGUI(rect);
    if (!inputFoldout.IsFolded()) {
        Editor::BeginIndent(rect, Editor::LargeGap());
        for (auto &input: inputs) {
            input.OnEditorGUI(rect);
        }
        if (GuiButton({rect.x, rect.y, rect.width, Editor::TextSize() * 1.2f}, "Add input")) {
            inputs.push_back({"New input", nullptr});
        }
        rect.y += Editor::TextSize() * 1.2f + Editor::SmallGap();
        Editor::EndIndent(rect, Editor::LargeGap());
    }

    // outputs

    debugFoldout.OnEditorGUI(rect);
    if (debugFoldout.IsFolded())
        return;

    // debug: position, size
    Editor::BeginIndent(rect, Editor::LargeGap());
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
             TextFormat("Position: [%i, %i]", (int) position.x, (int) position.y));
    rect.y += Editor::TextSize() + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
             TextFormat("Size: [%i, %i]", (int) size.x, (int) size.y));
    rect.y += Editor::TextSize() + Editor::SmallGap();
    Editor::EndIndent(rect, Editor::LargeGap());
}
