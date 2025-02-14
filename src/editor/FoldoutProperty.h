#ifndef FOLOUTPROPERTY_H
#define FOLOUTPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class FoldoutProperty {
public:
    explicit FoldoutProperty(std::string label, const bool folded = false) : label(std::move(label)), folded(folded) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const {
        return folded;
    }

    void SetLabel(const char * str) {
        label = str;
    }

private:
    std::string label;
    bool folded;
};


#endif //FOLOUTPROPERTY_H
