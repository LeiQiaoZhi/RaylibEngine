#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include "../utils/ShaderParam.h"


class MaterialProperty {
public:
    explicit MaterialProperty(Model *model, const int materialIndex)
        : model(model), materialIndex(materialIndex) {
        highlightShader = LoadShader(
            (std::string(INTERNAL_ASSET_DIR) + "/shaders/default.vert").c_str(),
            (std::string(INTERNAL_ASSET_DIR) + "/shaders/highlight.frag").c_str());
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

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
    Model *model = nullptr;
    char filename[256] = "";
    int materialIndex = 0;
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
