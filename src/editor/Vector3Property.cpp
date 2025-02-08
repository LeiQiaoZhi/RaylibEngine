//
// Created by Qiaozhi Lei on 2/7/25.
//

#include <raygui.h>
#include "Vector3Property.h"
#include "Editor.h"
#include "../raylib-utils/RaylibUtils.h"

void Vector3Property::OnEditorGUI(Rectangle &rect) {
    DrawText(label.c_str(), rect.x, rect.y, Editor::TextSize(), Editor::TextColor());
    // const int labelWidth = MeasureText(label.c_str(), Editor::TextSize()) + Editor::LargeGap();
    const int labelWidth = Editor::TextSize() * 5 + Editor::LargeGap();
    rect.width -= labelWidth;
    rect.x += labelWidth;

    Rectangle positionRect = {rect.x, rect.y, rect.width / 3.0f, Editor::TextSize() * 1.0f};
    for (int i = 0; i < 3; i++) {
        const int sublabelWidth = MeasureText(subLabels[i].c_str(), Editor::TextSize()) + Editor::SmallGap();
        positionRect.width = rect.width / 3.0f  - sublabelWidth;
        positionRect.x += sublabelWidth;
        if (GuiValueBoxFloat(positionRect, subLabels[i].c_str(), valueBoxTextValues[i],
                             RaylibUtils::Vector3Get(vector3, i), editModes[i])) {
            editModes[i] = !editModes[i];
        }
        positionRect.x += rect.width / 3.0f - sublabelWidth;
    }
    rect.x -= labelWidth;
    rect.width += labelWidth;

    rect.y += Editor::TextSize() + Editor::SmallGap();
}

float Vector3Property::GetEditorHeight() const {
    return Editor::TextSize() * 1 + Editor::SmallGap();
}
