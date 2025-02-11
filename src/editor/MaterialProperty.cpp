#include <raygui.h>
#include "MaterialProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"

void MaterialProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, path, 256, pathEditMode)) {
        pathEditMode = !pathEditMode;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load Material")) {
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
        // TODO: test extension

        // unload first
        UnloadMaterial(model->materials[materialIndex]);
        model->materials[materialIndex] = RaylibUtils::LoadMaterialFromFile(fullPath.c_str());
        statusText = "Material loaded";
        statusWarning = false;
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

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
