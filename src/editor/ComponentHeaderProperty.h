#ifndef COMPONENTHEADERPROPERTY_H
#define COMPONENTHEADERPROPERTY_H
#include <string>

#include "raylib.h"
#include "../raylib-utils/RaylibUtils.h"


class ComponentHeaderProperty {
public:
    explicit ComponentHeaderProperty(std::string label) : label(std::move(label)) {
    }
    void OnEditorGUI(Rectangle& rect);
    float GetEditorHeight() const;
    bool IsFolded() const {
        return folded;
    }

private:
    std::string label;
    bool folded = false;
};


#endif //COMPONENTHEADERPROPERTY_H
