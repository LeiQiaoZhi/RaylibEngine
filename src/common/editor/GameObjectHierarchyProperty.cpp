#include <raygui.h>
#include "GameObjectHierarchyProperty.h"
#include "Editor.h"
#include "../GCM/GameObject.h"

void GameObjectHierarchyProperty::OnEditorGUI(Rectangle &rect) {
    // Title
    const float originalY = rect.y;
    const char *title = gameObject->GetName();
    const int icon = gameObject->GetChildren().empty()
                         ? ICON_NONE
                         : folded
                               ? ICON_ARROW_RIGHT_FILL
                               : ICON_ARROW_DOWN_FILL;

    const char *iconText = GuiIconText(icon, "");

    // Highlight if selected
    const Rectangle titleRect = {rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f};
    if (gameObject->scene->selectedGameObjectUID == gameObject->GetUID()) {
        DrawRectangleRec(titleRect, Fade(Editor::ContrastBlackOrWhite(Editor::TextColor()), 0.5f));
    }

    // Title UI
    const Rectangle foldButtonRect = {
        rect.x, rect.y, Editor::TextSize() + Editor::SmallGap() * 1.0f, Editor::TextSize() * 1.0f
    };
    if (GuiLabelButton(foldButtonRect, iconText)) {
        folded = !folded;
    }

    // Hover -- show action buttons
    Rectangle titleLabelRect = {
        rect.x + foldButtonRect.width, rect.y, rect.width - foldButtonRect.width, Editor::TextSize() * 1.0f
    };
    if (CheckCollisionPointRec(GetMousePosition(), titleLabelRect)) {
        const float buttonWidth = Editor::TextSize() * 1.2f;
        Rectangle endRect = {rect.x + rect.width - buttonWidth, rect.y, buttonWidth, Editor::TextSize() * 1.0f};

        if (GuiLabelButton(endRect, GuiIconText(ICON_FILE_ADD, ""))) {
            gameObject->AddChild("New GameObject");
        }
        endRect.x -= buttonWidth + Editor::MediumGap();
        titleLabelRect.width -= buttonWidth + Editor::MediumGap();
        if (GuiLabelButton(endRect, GuiIconText(ICON_CROSS, ""))) {
            showMessageBox = true;
        }
        titleLabelRect.width -= buttonWidth + Editor::MediumGap();
    }

    if (GuiLabelButton(titleLabelRect, title)) {
        if (gameObject->scene->selectedGameObjectUID == gameObject->GetUID())
            gameObject->scene->selectedGameObjectUID = -1;
        else
            gameObject->scene->selectedGameObjectUID = gameObject->GetUID();
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // Remove game object message box
    if (showMessageBox) {
        Rectangle messageBoxRect = {
            rect.x, rect.y,
            rect.width, Editor::TextSize() * 4.0f + Editor::SmallGap()
        };
        int result = GuiMessageBox(
            messageBoxRect,
            GuiIconText(ICON_FILE_DELETE, TextFormat("Remove %s?", gameObject->GetName())), nullptr, "Yes;No");
        if (result >= 0) // 0: Close, 1: Yes, 2: No
            showMessageBox = false;
        if (result == 1) {
            if (GameObject *parent = gameObject->GetParent(); parent != nullptr)
                parent->RemoveChild(gameObject);
            return;
        }

        rect.y += messageBoxRect.height + Editor::SmallGap();
    }

    // Children
    if (folded) {
        height = rect.y - originalY;
        return;
    }

    for (auto *child: gameObject->GetChildren()) {
        if (child == nullptr) continue;

        if (children.find(child->GetUID()) == children.end()) {
            children[child->GetUID()] = std::make_unique<GameObjectHierarchyProperty>(child);
        }
        GameObjectHierarchyProperty *GameObjectHierarchyProperty = children[child->GetUID()].get();

        float indent = Editor::SmallGap() * 4;
        Editor::BeginIndent(rect, indent);

        GameObjectHierarchyProperty->OnEditorGUI(rect);

        Editor::EndIndent(rect, indent);

        rect.height -= GameObjectHierarchyProperty->GetEditorHeight();
    }
    height = rect.y - originalY;
}


float GameObjectHierarchyProperty::GetEditorHeight() const {
    return height;
}
