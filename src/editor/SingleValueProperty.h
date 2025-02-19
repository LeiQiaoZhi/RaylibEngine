#ifndef SINGLEVALUEPROPERTY_H
#define SINGLEVALUEPROPERTY_H
#include <string>
#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include <cstdio>
#include "Editor.h"

// Generic value property editor for float or int
template<typename T, bool hasSlider>
class SingleValueProperty {
public:
    static constexpr bool isInt = std::is_same_v<T, int>;

    explicit SingleValueProperty(T *value, std::string label, T min = 0, T max = 1)
        : label(std::move(label)), value(value), minValue(min), maxValue(max) {
        SetValueBoxText();
    }

    void SetValueBoxText() {
        if constexpr (isInt) {
            snprintf(valueBoxTextValue, sizeof(valueBoxTextValue), "%i", *value);
        } else {
            if (*value - floorf(*value) < 0.0001f) {
                snprintf(valueBoxTextValue, sizeof(valueBoxTextValue), "%.0f", *value);
            } else {
                snprintf(valueBoxTextValue, sizeof(valueBoxTextValue), "%.2f", *value);
            }
        }
    }

    void OnEditorGUI(Rectangle &rect) {
        const int labelWidth = Editor::LabelWidth();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, label.c_str());

        Editor::BeginIndent(rect, labelWidth);
        const Rectangle positionRect = {rect.x, rect.y, rect.width / 3 - Editor::SmallGap(), Editor::TextSize() * 1.0f};
        if constexpr (isInt) {
            if (GuiValueBox(positionRect, nullptr, valueBoxTextValue, value, editMode)) {
                editMode = !editMode;
            }
        } else {
            if (GuiValueBoxFloat(positionRect, nullptr, valueBoxTextValue, value, editMode)) {
                editMode = !editMode;
            }
        }
        if constexpr (hasSlider) {
            if (GuiSlider({
                              rect.x + rect.width / 3 + Editor::SmallGap(), rect.y,
                              rect.width * 2 / 3 - Editor::SmallGap() * 2,
                              Editor::TextSize() * 1.0f
                          },
                          nullptr, nullptr, value, minValue, maxValue)) {
                SetValueBoxText();
            }
        }
        if (!editMode) {
            SetValueBoxText(); // sync text with value, in case it was changed elsewhere
        }

        Editor::EndIndent(rect, labelWidth);

        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    float GetEditorHeight() const {
        return Editor::TextSize() * 1 + Editor::SmallGap();
    }

private:
    std::string label;
    T *value = nullptr;
    bool editMode = false;
    char valueBoxTextValue[32] = {0}; // For storing text representation of each component
    T minValue;
    T maxValue;
};

using IntProperty = SingleValueProperty<int, false>;
using FloatProperty = SingleValueProperty<float, false>;
using IntSlider = SingleValueProperty<int, true>;
using FloatSlider = SingleValueProperty<float, true>;


#endif //SINGLEVALUEPROPERTY_H
