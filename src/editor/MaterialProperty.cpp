#include <raygui.h>
#include "MaterialProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../utils/Utils.h"

void MaterialProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    const char *label = TextFormat("[%i] Material", meshIndex);
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
    const float defaultWidth = Editor::TextWidth("Default") + Editor::SmallGap() * 2;
    const float highlightWidth = Editor::TextWidth("Highlight") + Editor::SmallGap() * 2;
    const float loadWidth = rect.width - defaultWidth - highlightWidth;
    if (GuiButton(Rectangle{rect.x, rect.y, loadWidth, Editor::TextSize() * 1.5f}, "Load Material")) {
        LoadMaterialFromFile(filename);
    }

    // highlight
    Editor::BeginIndent(rect, loadWidth);
    if (GuiButton(Rectangle{rect.x, rect.y, highlightWidth, Editor::TextSize() * 1.5f},
                  highlighted ? "Cancel" : "Highlight")) {
        if (model == nullptr) {
            statusText = "Model is null";
            statusWarning = true;
        } else {
            highlighted = !highlighted;
            statusText = highlighted ? "Material highlighted" : "Material unhighlighted";
            statusWarning = false;
            if (highlighted) {
                originalShader = model->materials[model->meshMaterial[meshIndex]].shader;
                model->materials[model->meshMaterial[meshIndex]].shader = highlightShader;
            } else {
                model->materials[model->meshMaterial[meshIndex]].shader = originalShader;
            }
        }
    }

    // load default
    Editor::BeginIndent(rect, highlightWidth);
    if (GuiButton(Rectangle{rect.x, rect.y, defaultWidth, Editor::TextSize() * 1.5f}, "Default")) {
        if (model == nullptr) {
            statusText = "Model is null";
            statusWarning = true;
            return;
        }
        UnloadMaterial(model->materials[model->meshMaterial[meshIndex]]);

        model->materials[model->meshMaterial[meshIndex]] = LoadMaterialDefault();
        statusText = "Material set to default";
        statusWarning = false;

        highlighted = false;
        originalShader = model->materials[model->meshMaterial[meshIndex]].shader;
    }
    Editor::EndIndent(rect, highlightWidth);
    Editor::EndIndent(rect, loadWidth);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // load shader
    if (GuiButton(Rectangle{rect.x, rect.y, rect.width / 2, Editor::TextSize() * 1.5f}, "Load Shader Only")) {
        LoadShaderFromMaterialFile(filename);
    }
    if (GuiButton(Rectangle{rect.x + rect.width/2, rect.y, rect.width / 2, Editor::TextSize() * 1.5f}, "Load Shader For All")) {
        const Shader shader = LoadShaderFromMaterialFile(filename);
        if (!statusWarning) {
            for (int i = 0; i < model->meshCount; i++) {
                model->materials[model->meshMaterial[i]].shader = shader;
            }
        }
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    height = rect.y - originalY;
}

float MaterialProperty::GetEditorHeight() const {
    return height;
}

Shader MaterialProperty::LoadShaderFromMaterialFile(const char *filename) {
    Shader shader = {0};
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
        const auto fullPath = ASSET_DIR + std::string("/materials/") + std::string(filename);
        const Material material = RaylibUtils::LoadMaterialFromFile(fullPath.c_str(), shaderParams);
        shader = material.shader;
        model->materials[model->meshMaterial[meshIndex]].shader = shader;
        statusText = "Shader loaded";
        statusWarning = false;
    }
    return shader;
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
        UnloadMaterial(model->materials[model->meshMaterial[meshIndex]]);

        const auto fullPath = ASSET_DIR + std::string("/materials/") + std::string(filename);
        model->materials[model->meshMaterial[meshIndex]] = RaylibUtils::LoadMaterialFromFile(fullPath.c_str(), shaderParams);
        statusText = "Material loaded";
        statusWarning = false;

        currentMaterialFilename = filename;
    }
}

void MaterialProperty::SendParamsToShader() const {
    const Shader &shader = model->materials[model->meshMaterial[meshIndex]].shader;
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
    j["inputText"] = filename; // what is displayed
    j["path"] = currentMaterialFilename; // what is last loaded
    return j;
}

void MaterialProperty::FromJson(const nlohmann::json &json) {
    if (json.contains("inputText"))
        SetInputText(json["inputText"].get<std::string>().c_str());
    else
        SetInputText(json["path"].get<std::string>().c_str());
    currentMaterialFilename = json.value("path", "");
    if (currentMaterialFilename != "") {
        LoadMaterialFromFile(currentMaterialFilename.c_str());
    }
}
