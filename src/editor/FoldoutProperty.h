#ifndef FOLOUTPROPERTY_H
#define FOLOUTPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class FoldoutProperty {
public:
    explicit FoldoutProperty(std::string label, bool foled = false) : label(std::move(label)), folded(foled) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const {
        return folded;
    }

private:
    std::string label;
    bool folded;
};


#endif //FOLOUTPROPERTY_H
