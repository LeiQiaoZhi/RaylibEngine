#include <raygui.h>
#include "LuaProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"

void LuaProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    const float labelWidth = Editor::LabelWidth();
    if (value.get_type() == sol::type::lua_nil) {
        const std::string label = key + " is nil";
        GuiLabel(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, label.c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
    } else if (value.get_type() == sol::type::boolean) {
        GuiLabel(Rectangle{rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, key.c_str());
        Editor::BeginIndent(rect, labelWidth);
        if (GuiCheckBox(Rectangle{rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f},
                        nullptr, &boolValue)) {
            propsTable[key] = boolValue;
        }
        Editor::EndIndent(rect, labelWidth);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    } else if (value.get_type() == sol::type::number) {
        numberProperty.OnEditorGUI(rect);
        propsTable[key] = numberValue;
    } else if (value.get_type() == sol::type::string) {
        GuiLabel(Rectangle{rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, key.c_str());
        Editor::BeginIndent(rect, labelWidth);
        if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                       stringValue, 256, stringEditMode)) {
            stringEditMode = !stringEditMode;
            if (!stringEditMode) {
                propsTable[key] = std::string(stringValue);
            }
        }
        Editor::EndIndent(rect, labelWidth);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    height = rect.y - originalY;
}

float LuaProperty::GetEditorHeight() const {
    return height;
}

void LuaProperty::FromJson(const nlohmann::json &json) {
}

nlohmann::json LuaProperty::ToJson() const {
    json j;
    return j;
}
