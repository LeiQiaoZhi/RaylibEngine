#ifndef COLORPROPERTY_H
#define COLORPROPERTY_H
#include <string>

#include "FoldoutProperty.h"
#include "raylib.h"
#include "../utils/RaylibUtils.h"


class ColorProperty {
public:
    explicit ColorProperty(Color *color, std::string label, bool folded = false)
        : label(std::move(label)), color(color), foldoutProperty(this->label, folded) {
        colorHSV = ColorToHSV(*color);
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

private:
    std::string label;
    Vector3 colorHSV = {0, 0, 0};
    Color *color = nullptr;
    FoldoutProperty foldoutProperty;
    float editorHeight = 0;
    float alpha = 1.0f;
};


#endif //COLORPROPERTY_H
