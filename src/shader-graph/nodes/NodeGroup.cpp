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
    if (!initialized && !isSelectionGroup) {
        context.groupToDelete = this;
        return;
    }

    position = minPoint - Vector2{Editor::SmallGap(), Editor::SmallGap()};
    size = maxPoint - minPoint + Vector2{Editor::SmallGap() * 2, Editor::SmallGap() * 2};

    if (isSelectionGroup) {
        DrawRectangleRec(
            {position.x, position.y, size.x, size.y},
            Fade(BLUE, 0.1f));
    } else {
        const float textWidth = std::min(size.x * context.camera.zoom, 150.0f);
        Vector2 textSize = Vector2({textWidth, Editor::TextSizeF()});
        textSize.y = std::max(textSize.y, Editor::TextSizeF());
        position.y -= (Editor::TextSizeF() + Editor::MediumGap()) / context.camera.zoom;
        size.y += (Editor::TextSizeF() + Editor::MediumGap()) / context.camera.zoom;


        DrawRectangleRec(
            {position.x, position.y, size.x, size.y},
            Fade(GRAY, 0.2f));

        EndMode2D();
        // set gui text size
        float originalTextSize = Editor::TextSizeF();
        GuiSetStyle(DEFAULT, TEXT_SIZE, textSize.y);
        const Vector2 guiPos = GetWorldToScreen2D(position, context.camera);

        const float buttonWidth = Editor::TextSizeF() + Editor::SmallGap() * 2.;
        textSize.x -= buttonWidth * 2;

        if (GuiLabelButton({
                               guiPos.x + Editor::SmallGap(),
                               guiPos.y + Editor::SmallGap(), buttonWidth, textSize.y
                           }, GuiIconText(ICON_CURSOR_CLASSIC, ""))) {
            context.SelectionGroup()->Clear();
            for (auto &node: nodeUIDs) {
                Node *n = context.FindNodeByUID(node);
                if (n == nullptr) continue;
                context.SelectionGroup()->AddNode(n);
            }
        }

        if (GuiLabelButton({
                               guiPos.x + size.x * context.camera.zoom - buttonWidth + Editor::SmallGap(),
                               guiPos.y + Editor::SmallGap(), buttonWidth, textSize.y
                           }, GuiIconText(ICON_CROSS, ""))) {
            context.groupToDelete = this;
        }

        const Rectangle textRect = {
            guiPos.x + buttonWidth, guiPos.y + Editor::SmallGap(), textSize.x, textSize.y
        };
        // no border
        float originalBorderWidth = GuiGetStyle(DEFAULT, BORDER_WIDTH);
        GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);
        if (GuiTextBox(textRect, const_cast<char *>(name.c_str()), 256, nameEditMode)) {
            nameEditMode = !nameEditMode;
        }
        GuiSetStyle(DEFAULT, BORDER_WIDTH, originalBorderWidth);
        GuiSetStyle(DEFAULT, TEXT_SIZE, originalTextSize);
        BeginMode2D(context.camera);
    }
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
        std::cout << "Dragging group: " << name << std::endl;
        for (auto &node: nodeUIDs) {
            Node *n = context.FindNodeByUID(node);
            if (n == nullptr) continue;
            n->position += context.mouseDragState.delta / context.camera.zoom;
        }
    }
}

void NodeGroup::OnEditorGUI(Rectangle &rect, Context &context) {
    const float originalY = rect.y;

    const char *deleteLabel = GuiIconText(ICON_CROSS_SMALL, "");
    const float deleteWidth = Editor::TextSizeF() + Editor::SmallGap() * 2.;
    GuiLabel({rect.x, rect.y, rect.width - deleteWidth, Editor::TextSize() * 1.0f},
             isSelectionGroup ? "Selection Group" : name.c_str());

    if (context.FinalNode()->uid != uid) {
        if (GuiLabelButton({
                               rect.x + rect.width - deleteWidth + Editor::SmallGap(), rect.y,
                               deleteWidth, Editor::TextSizeF()
                           },
                           deleteLabel)) {
            context.groupToDelete = this;
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
