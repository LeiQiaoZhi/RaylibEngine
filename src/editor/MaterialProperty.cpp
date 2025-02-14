#include <raygui.h>
#include "MaterialProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../utils/Utils.h"

void MaterialProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    const char* label = TextFormat("[%i] Material", materialIndex);
    const float labelWidth = MeasureText(label, Editor::TextSize()) + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, label);

    // path
    Editor::BeginIndent(rect, labelWidth + Editor::SmallGap());
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, path, 256, pathEditMode)) {
        pathEditMode = !pathEditMode;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
    Editor::EndIndent(rect, labelWidth + Editor::SmallGap());

    // load material
    const float buttonWidth = rect.width - MeasureText("Default", Editor::TextSize()) - Editor::SmallGap() * 2;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.5f}, "Load Material")) {
        if (path[0] == '\0') {
            statusText = "Path is empty";
            statusWarning = true;
            return;
        }
        if (model == nullptr) {
            statusText = "Model is null";
            statusWarning = true;
            return;
        }
        const auto fullPath = ASSET_DIR + std::string("/materials/") + std::string(path);
        if (!FileExists(fullPath.c_str())) {
            statusText = "File does not exist";
            statusWarning = true;
            return;
        }
        if (!Utils::EndsWith(path, ".mat.json")) {
            statusText = "File must be .mat.json";
            statusWarning = true;
            return;
        }

        UnloadMaterial(model->materials[materialIndex]);

        model->materials[materialIndex] = RaylibUtils::LoadMaterialFromFile(fullPath.c_str());
        statusText = "Material loaded";
        statusWarning = false;
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
    if (!statusText.empty()) {
        const char *text = statusWarning ? GuiIconText(ICON_WARNING, statusText.c_str()) : statusText.c_str();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, text);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    height = rect.y - originalY;
}

float MaterialProperty::GetEditorHeight() const {
    return height;
}
