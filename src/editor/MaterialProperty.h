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

    std::string GetMaterialFilename() {
        return currentMaterialFilename;
    }

    void SetInputText(const char *input) {
        std::strncpy(filename, input, sizeof(filename));
    }

    void LoadMaterialFromFile(const char* fullPath);

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

private:
    Model *model = nullptr;
    char filename[256] = "";
    int materialIndex = 0;
    std::string currentMaterialFilename = ""; // only change when loaded

    float height;
    std::string statusText = "";
    bool pathEditMode = false;
    bool statusWarning = false;
};


#endif //MATERIALPROPERTY_H
