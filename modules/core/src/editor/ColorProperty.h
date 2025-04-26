#ifndef COLORPROPERTY_H
#define COLORPROPERTY_H
#include <string>

#include "FoldoutProperty.h"
#include "raylib.h"
#include "../utils/RaylibUtils.h"


class ColorProperty {
public:
    explicit ColorProperty(Color *color, std::string label, const bool useAlpha = true, const bool folded = false)
        : label(std::move(label)), color(color), useAlpha(useAlpha), foldoutProperty(this->label, folded) {
        colorHSV = ColorToHSV(*color);
        alpha = color->a / 255.0f;
    }

    void UpdateColor(const Color &color) {
        colorHSV = ColorToHSV(color);
        alpha = color.a / 255.0f;
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

private:
    std::string label;
    Vector3 colorHSV = {0, 0, 0};
    Color *color = nullptr;
    FoldoutProperty foldoutProperty;
    float editorHeight = 0;
    bool useAlpha;
    float alpha = 1.0f;
};


#endif //COLORPROPERTY_H
