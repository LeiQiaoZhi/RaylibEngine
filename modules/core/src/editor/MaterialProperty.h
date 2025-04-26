#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include "../utils/ShaderParam.h"


class Component;

class MaterialProperty {
public:
    explicit MaterialProperty(Model *model, const int meshIndex, Component* parent)
        : model(model), meshIndex(meshIndex), parent(parent) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    Shader LoadShaderFromMaterialFile(const char *filename);

    void SetModel(Model *model) {
        this->model = model;
    }

    std::string GetMaterialFilename() {
        return currentMaterialFilename;
    }

    void SetInputText(const char *input) {
        std::strncpy(filename, input, sizeof(filename));
    }

    void LoadMaterialFromFile(const char *fullPath);

    void LoadMaterial() {
        LoadMaterialFromFile(filename);
    }

    void SendParamsToShader() const;

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

private:
    Component* parent = nullptr;
    Model *model = nullptr;
    char filename[256] = "";
    int meshIndex = 0;
    std::string currentMaterialFilename; // only change when loaded
    std::vector<ShaderParam> shaderParams;
    Shader highlightShader;
    Shader originalShader;
    bool highlighted = false;

    float height;
    std::string statusText = "";
    bool pathEditMode = false;
    bool statusWarning = false;
};


#endif //MATERIALPROPERTY_H
