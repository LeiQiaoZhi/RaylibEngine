#include <raygui.h>
#include "ComponentHeaderProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"

void ComponentHeaderProperty::OnEditorGUI(Rectangle &rect) {
    Color color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
    if (!*enabled) {
        color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_DISABLED));
    }
    DrawRectangleRec({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, color);

    const float checkboxWidth = Editor::TextSize() * 1.0f;
    const char *title = GuiIconText(folded ? ICON_ARROW_RIGHT_FILL : ICON_ARROW_DOWN_FILL, label.c_str());
    if (GuiLabelButton({rect.x, rect.y, rect.width - checkboxWidth, Editor::TextSize() * 1.0f}, title)) {
        folded = !folded;
    }
    Rectangle endRect = {rect.x + rect.width - checkboxWidth, rect.y, checkboxWidth, Editor::TextSize() * 1.0f};
    GuiCheckBox(endRect, nullptr, enabled);

    rect.y += Editor::TextSize() + Editor::SmallGap();
}

float ComponentHeaderProperty::GetEditorHeight() const {
    return Editor::TextSize() * 1 + Editor::SmallGap();
}
