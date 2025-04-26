#ifndef LUAPROPERTY_H
#define LUAPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include <sol/sol.hpp>

#include "SingleValueProperty.h"

class LuaProperty {
public:
    explicit LuaProperty(std::string key, sol::object value, sol::table propsTable)
        : key(std::move(key)), value(std::move(value)), propsTable(propsTable) {
        // Initialize value
        if (this->value.is<bool>()) {
            boolValue = this->value.as<bool>();
        } else if (this->value.get_type() == sol::type::number) {
            numberValue = this->value.as<float>();
            numberProperty = FloatProperty(&numberValue, this->key.c_str());
        } else if (this->value.get_type() == sol::type::string) {
            std::strncpy(stringValue, this->value.as<std::string>().c_str(), sizeof(stringValue));
        }
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    void FromJson(const nlohmann::json &json);

    nlohmann::json ToJson() const;

    std::string GetKey() const {
        return key;
    }

private:
    std::string key;
    sol::object value;
    sol::table propsTable;

    bool boolValue = false;
    char stringValue[256];
    bool stringEditMode = false;
    float numberValue = 0;
    FloatProperty numberProperty = FloatProperty(&numberValue, "");

    float height = 0;
};


#endif //LUAPROPERTY_H
