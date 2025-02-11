#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class MaterialProperty {
public:
    explicit MaterialProperty(Model *model, const int materialIndex)
        : model(model), materialIndex(materialIndex) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    void SetModel(Model * model) {
        this->model = model;
    }

private:
    Model *model = nullptr;
    char path[256] = "";
    int materialIndex = 0;

    float height;
    std::string statusText = "";
    bool pathEditMode = false;
    bool statusWarning = false;
};


#endif //MATERIALPROPERTY_H
