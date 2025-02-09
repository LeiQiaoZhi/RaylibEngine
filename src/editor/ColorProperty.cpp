#include <raygui.h>
#include "ColorProperty.h"
#include "Editor.h"

void ColorProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    const float dimension = rect.width * .5f;
    const Color textColor = Editor::BlackOrWhiteBasedOnBackground(*color);
    Color original = Editor::BeginTextColor(textColor);
    DrawRectangleRec({rect.x, rect.y, dimension, Editor::TextSize() * 1.0f}, *color);
    foldoutProperty.OnEditorGUI(rect);
    Editor::EndTextColor(original);
    if (foldoutProperty.IsFolded()) return;

    GuiColorPickerHSV({rect.x, rect.y, dimension, dimension}, nullptr, &colorHSV);
    const Color newColor = ColorFromHSV(colorHSV.x, colorHSV.y, colorHSV.z);
    color->r = newColor.r;
    color->g = newColor.g;
    color->b = newColor.b;
    color->a = newColor.a;
    rect.y += dimension + Editor::SmallGap();

    editorHeight = rect.y - originalY;
}

float ColorProperty::GetEditorHeight() const {
    return editorHeight;
}
