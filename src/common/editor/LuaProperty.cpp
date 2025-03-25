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
    // json: { value = ... }
    if (value.get_type() == sol::type::boolean) {
        boolValue = json["value"].get<bool>();
    } else if (value.get_type() == sol::type::number) {
        numberValue = json["value"].get<float>();
        numberProperty = FloatProperty(&numberValue, key.c_str());
    } else if (value.get_type() == sol::type::string) {
        std::strncpy(stringValue, json["value"].get<std::string>().c_str(), sizeof(stringValue));
    }
}

nlohmann::json LuaProperty::ToJson() const {
    json j;
    if (value.get_type() == sol::type::boolean) {
        j["value"] = boolValue;
    } else if (value.get_type() == sol::type::number) {
        j["value"] = numberValue;
    } else if (value.get_type() == sol::type::string) {
        j["value"] = std::string(stringValue);
    }
    return j;
}
