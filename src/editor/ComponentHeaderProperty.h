#ifndef COMPONENTHEADERPROPERTY_H
#define COMPONENTHEADERPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class ComponentHeaderProperty {
public:
    explicit ComponentHeaderProperty(std::string label, bool *enabled) : label(std::move(label)), enabled(enabled) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const {
        return folded;
    }

public:
    std::string label;

private:
    bool *enabled;
    bool folded = false;
};


#endif //COMPONENTHEADERPROPERTY_H
