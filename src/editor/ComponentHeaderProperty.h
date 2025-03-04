#ifndef COMPONENTHEADERPROPERTY_H
#define COMPONENTHEADERPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"

class Component;

class ComponentHeaderProperty {
public:
    explicit ComponentHeaderProperty(std::string label, bool *enabled, Component *component)
        : label(std::move(label)), component(component), enabled(enabled) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const;

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

public:
    std::string label;

private:
    Component* component;
    bool showMessageBox = false;
    bool *enabled;
    bool folded = false;

    std::string statusText;
    bool statusWarning = false;
};


#endif //COMPONENTHEADERPROPERTY_H
