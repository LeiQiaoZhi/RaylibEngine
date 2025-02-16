#ifndef VECTOR3PROPERTY_H
#define VECTOR3PROPERTY_H
#include <string>
#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include <cstdio>
#include "Editor.h"

// Generic vector property editor for Vector2 or Vector3
template<typename T>
class VectorProperty {
public:
    // Determine the number of components at compile time:
    static constexpr int n = std::is_same<T, Vector3>::value ? 3 : 2;

    explicit VectorProperty(T *vector, std::string label)
        : label(std::move(label)), vector(vector) {
        for (int i = 0; i < n; i++) {
            float value = *RaylibUtils::GetVectorComponent<T>(vector, i);
            if (value - floorf(value) < 0.0001f) {
                snprintf(valueBoxTextValues[i], sizeof(valueBoxTextValues[i]), "%.0f", value);
            } else {
                snprintf(valueBoxTextValues[i], sizeof(valueBoxTextValues[i]), "%.2f", value);
            }
        }
    }

    void SetValueBoxText() {
        for (int i = 0; i < n; i++) {
            SetValueBoxText(i);
        }
    }

    void SetValueBoxText(int i) {
        float value = *RaylibUtils::GetVectorComponent<T>(vector, i);
        if (value - floorf(value) < 0.0001f) {
            snprintf(valueBoxTextValues[i], sizeof(valueBoxTextValues[i]), "%.0f", value);
        } else {
            snprintf(valueBoxTextValues[i], sizeof(valueBoxTextValues[i]), "%.2f", value);
        }
    }

    void OnEditorGUI(Rectangle &rect) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, label.c_str());
        const int labelWidth = Editor::TextSize() * 5 + Editor::LargeGap();
        rect.width -= labelWidth;
        rect.x += labelWidth;

        Rectangle positionRect = {rect.x, rect.y, rect.width / n, Editor::TextSize() * 1.0f};
        for (int i = 0; i < n; i++) {
            const float subLabelWidth = Editor::TextWidth(subLabels[i].c_str()) + Editor::SmallGap();

            GuiLabel({positionRect.x, positionRect.y, subLabelWidth, positionRect.height}, subLabels[i].c_str());
            positionRect.width = rect.width / n - subLabelWidth - Editor::SmallGap();
            positionRect.x += subLabelWidth;
            if (GuiValueBoxFloat(positionRect, nullptr, valueBoxTextValues[i],
                                 RaylibUtils::GetVectorComponent(vector, i), editModes[i])) {
                editModes[i] = !editModes[i];
            }
            positionRect.x += rect.width / n - subLabelWidth;

            if (!editModes[i]) {
                SetValueBoxText(i); // sync text with value, in case it was changed elsewhere
            }
        }
        rect.x -= labelWidth;
        rect.width += labelWidth;

        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    float GetEditorHeight() const {
        return Editor::TextSize() * 1 + Editor::SmallGap();
    }

private:
    std::string label;
    std::string subLabels[3] = {"X", "Y", "Z"};
    T *vector = nullptr;
    bool editModes[n] = {false}; // For tracking editing mode per component
    char valueBoxTextValues[n][32] = {{0}}; // For storing text representation of each component
};

using Vector3Property = VectorProperty<Vector3>;
using Vector2Property = VectorProperty<Vector2>;


#endif //VECTOR3PROPERTY_H
