#include <raygui.h>
#include "ColorProperty.h"
#include "Editor.h"

void ColorProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    const float dimension = rect.width * .5f;
    const Color textColor = Editor::ContrastBlackOrWhite(*color);
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
    rect.y += dimension + Editor::SmallGap();

    if (useAlpha) {
        GuiColorBarAlpha({rect.x, rect.y, dimension, Editor::TextSize() * 1.0f}, nullptr, &alpha);
        color->a = alpha * 255;
        rect.y += Editor::TextSize() + Editor::SmallGap();
    } else {
        color->a = 255;
    }

    editorHeight = rect.y - originalY;
}

float ColorProperty::GetEditorHeight() const {
    return editorHeight;
}
