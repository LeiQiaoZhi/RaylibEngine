#include <raygui.h>
#include "DropdownProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../utils/Utils.h"

void DropdownProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    if (GuiDropdownBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, optionsStr.c_str(),
                       &selectedIndex, editMode)) {
        editMode = !editMode;
        justSelected = true;
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    height = rect.y - originalY;
}

float DropdownProperty::GetEditorHeight() const {
    return editMode
               ? height + options.size() * (Editor::TextSize() * 1.5f +
                                            GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_SPACING))
               : height;
}

nlohmann::json DropdownProperty::ToJson() const {
    nlohmann::json j;
    return j;
}

void DropdownProperty::FromJson(const nlohmann::json &json) {
}
