#include <raygui.h>
#include "MaterialProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../utils/Utils.h"

void MaterialProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    const char *label = TextFormat("[%i] Material", materialIndex);
    const float labelWidth = Editor::TextWidth(label) + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, label);

    // path
    Editor::BeginIndent(rect, labelWidth + Editor::SmallGap());
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, filename, 256, pathEditMode)) {
        pathEditMode = !pathEditMode;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
    Editor::EndIndent(rect, labelWidth + Editor::SmallGap());

    // load material
    const float buttonWidth = rect.width - Editor::TextWidth("Default") - Editor::SmallGap() * 2;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.5f}, "Load Material")) {
        LoadMaterialFromFile(filename);
    }

    // load default
    Editor::BeginIndent(rect, buttonWidth);
    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Default")) {
        if (model == nullptr) {
            statusText = "Model is null";
            statusWarning = true;
            return;
        }
        UnloadMaterial(model->materials[materialIndex]);
        model->materials[materialIndex] = LoadMaterialDefault();
        statusText = "Material set to default";
        statusWarning = false;
    }
    Editor::EndIndent(rect, buttonWidth);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    height = rect.y - originalY;
}

float MaterialProperty::GetEditorHeight() const {
    return height;
}

void MaterialProperty::LoadMaterialFromFile(const char *filename) {
    const auto fullPath = ASSET_DIR + std::string("/materials/") + std::string(filename);
    if (filename[0] == '\0') {
        statusText = "Path is empty";
        statusWarning = true;
    } else if (model == nullptr) {
        statusText = "Model is null";
        statusWarning = true;
    } else if (!FileExists(fullPath.c_str())) {
        statusText = "File does not exist";
        statusWarning = true;
    } else if (!Utils::EndsWith(filename, ".mat.json")) {
        statusText = "File must be .mat.json";
        statusWarning = true;
    } else {
        UnloadMaterial(model->materials[materialIndex]);

        const auto fullPath = ASSET_DIR + std::string("/materials/") + std::string(filename);
        model->materials[materialIndex] = RaylibUtils::LoadMaterialFromFile(fullPath.c_str(), shaderParams);
        statusText = "Material loaded";
        statusWarning = false;

        currentMaterialFilename = filename;
    }
}

void MaterialProperty::SendParamsToShader() const {
    const Shader &shader = model->materials[materialIndex].shader;
    for (ShaderParam param: shaderParams) {
        const int loc = GetShaderLocation(shader, param.name.c_str());
        const void *value = nullptr;
        if (const auto vec = std::get_if<Vector2>(&param.value)) {
            const float array[2] = {vec->x, vec->y};
            value = array;
        }
        if (const auto vec = std::get_if<Vector3>(&param.value)) {
            const float array[3] = {vec->x, vec->y, vec->z};
            value = array;
        }
        if (const auto vec = std::get_if<Vector4>(&param.value)) {
            const float array[4] = {vec->x, vec->y, vec->z, vec->w};
            value = array;
        }
        if (const auto val = std::get_if<float>(&param.value)) {
            value = val;
        }
        if (const auto val = std::get_if<int>(&param.value)) {
            value = val;
        }
        if (value != nullptr)
            SetShaderValue(shader, loc, value, param.type);
    }
}

nlohmann::json MaterialProperty::ToJson() const {
    nlohmann::json j;
    j["path"] = currentMaterialFilename;
    return j;
}

void MaterialProperty::FromJson(const nlohmann::json &json) {
    SetInputText(json["path"].get<std::string>().c_str());
    if (filename[0] != '\0') {
        LoadMaterialFromFile(filename);
    }
}
