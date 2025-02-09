#include <raygui.h>
#include "FoldoutProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"

void FoldoutProperty::OnEditorGUI(Rectangle &rect) {
    const char* title = GuiIconText(folded ? ICON_ARROW_RIGHT_FILL : ICON_ARROW_DOWN_FILL, label.c_str());
    if (GuiLabelButton({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, title)) {
        folded = !folded;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
}

float FoldoutProperty::GetEditorHeight() const {
    return Editor::TextSize() * 1 + Editor::SmallGap();
}
