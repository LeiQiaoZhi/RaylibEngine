#include <raygui.h>
#include "MaterialProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"

void MaterialProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, path, 256, pathEditMode)) {
        pathEditMode = !pathEditMode;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load Material")) {
        // TODO: load material from file
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    if (!statusText.empty()) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, statusText.c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    height = rect.y - originalY;
}

float MaterialProperty::GetEditorHeight() const {
    return height;
}
