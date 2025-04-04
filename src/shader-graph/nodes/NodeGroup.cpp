//
// Created by Qiaozhi Lei on 4/3/25.
//

#include "NodeGroup.h"

void NodeGroup::OnDraw(Context &context) {
    bool initialized = false;
    Vector2 minPoint;
    Vector2 maxPoint;
    for (const auto &node: nodeUIDs) {
        const Node *n = context.FindNodeByUID(node);
        if (n == nullptr) continue;
        if (!initialized) {
            initialized = true;
            minPoint = n->position;
            maxPoint = n->position + n->size;
        } else {
            minPoint = Vector2Min(minPoint, n->position);
            maxPoint = Vector2Max(maxPoint, n->position + n->size);
        }
    }

    position = minPoint - Vector2{Editor::SmallGap(), Editor::SmallGap()};
    size = maxPoint - minPoint + Vector2{Editor::SmallGap() * 2, Editor::SmallGap() * 2};

    DrawRectangleRec(
        {position.x, position.y, size.x, size.y},
        Fade(BLUE, 0.1f));
}

void NodeGroup::Update(Context &context) {
    const MouseDragState dragState = context.mouseDragState;
    const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
    const bool wasHovered =
            CheckCollisionPointRec(startMouseWorldPos, Rectangle{position.x, position.y, size.x, size.y});

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

void NodeGroup::Resolve(Context &context) {
    // resolve dragging
    if (dragging && context.interactionState == InteractionState::Dragging) {
        for (auto &node: nodeUIDs) {
            Node *n = context.FindNodeByUID(node);
            if (n == nullptr) continue;
            n->position += context.mouseDragState.delta;
        }
    }
}

void NodeGroup::OnEditorGUI(Rectangle &rect, Context &context) {
    const float originalY = rect.y;

    // highlight when selected
    if (context.SelectionGroup()->Contains(this)) {
        DrawRectangleRec({rect.x, rect.y, rect.width, height}, Fade(WHITE, 0.1f));
    }

    const char *deleteLabel = "X";
    const float deleteWidth = Editor::TextWidth(deleteLabel) + Editor::SmallGap() * 2.;
    GuiLabel({rect.x, rect.y, rect.width - deleteWidth, Editor::TextSize() * 1.0f}, name.c_str());

    if (context.FinalNode()->uid != uid) {
        if (GuiLabelButton({
                               rect.x + rect.width - deleteWidth + Editor::SmallGap(), rect.y,
                               deleteWidth, Editor::TextSizeF()
                           },
                           deleteLabel)) {
            context.nodeToDelete = this;
        }
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded()) {
        // debug: uid, position, size
        Editor::BeginIndent(rect, Editor::LargeGap());
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, TextFormat("UID: %d", uid));
        rect.y += Editor::TextSize() + Editor::SmallGap();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                 TextFormat("Position: [%i, %i]", (int) position.x, (int) position.y));
        rect.y += Editor::TextSize() + Editor::SmallGap();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                 TextFormat("Size: [%i, %i]", (int) size.x, (int) size.y));
        rect.y += Editor::TextSize() + Editor::SmallGap();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                 TextFormat("Preview Output Index: %d", previewOutputIndex));
        rect.y += Editor::TextSize() + Editor::SmallGap();
        Editor::EndIndent(rect, Editor::LargeGap());
    }

    height = rect.y - originalY;
}
