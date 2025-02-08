#include <raygui.h>
#include "ComponentHeaderProperty.h"
#include "Editor.h"
#include "../raylib-utils/RaylibUtils.h"

void ComponentHeaderProperty::OnEditorGUI(Rectangle &rect) {
    GuiDummyRec({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, "");
    if (GuiLabelButton({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, label.c_str())) {
        folded = !folded;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
}

float ComponentHeaderProperty::GetEditorHeight() const {
    return Editor::TextSize() * 1 + Editor::SmallGap();
}
