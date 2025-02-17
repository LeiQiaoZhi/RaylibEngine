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

public:
    std::string label;

private:
    Component* component;
    bool showMessageBox;
    bool *enabled;
    bool folded = false;

    std::string statusText;
    bool statusWarning;
};


#endif //COMPONENTHEADERPROPERTY_H
