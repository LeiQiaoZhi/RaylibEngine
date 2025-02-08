#include "ModelComponent.h"

#include <iostream>
#include <ostream>

#include "raylib.h"
#include "raymath.h"
#include "../../editor/Editor.h"
#include "../../GCM/GameObject.h"

void ModelComponent::OnEditorGUI(Rectangle &rect) {
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    // file name
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width - Editor::TextSize() * 2, Editor::TextSize() * 1.0f},
                   filename, 32, editMode)) {
        editMode = !editMode;
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load Model")) {
        const std::string path = std::string(PROJECT_DIR) + "/resources/" + filename;
        LoadModelFromFile(path);
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    if (!warningText.empty()) {
        DrawText(warningText.c_str(), rect.x, rect.y, Editor::TextSize(), YELLOW);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }
}

float ModelComponent::GetEditorHeight() const {
    return Editor::TextSize() * 3.5 + Editor::SmallGap() * 3 +
           (warningText.empty() ? 0 : Editor::TextSize() + Editor::SmallGap());
}

void ModelComponent::OnDraw(Scene *scene) const {
    if (model == nullptr) return;

    // TODO: transform
    model->transform = gameObject->GetTransform()->GetTransformMatrix();
    DrawModel(*model, {0, 0, 0}, 1.0f, WHITE);
}

void ModelComponent::OnDrawGizmos(Scene *scene) const {
}

void ModelComponent::OnDrawGizmosSelected(Scene *scene) const {
    DrawBoundingBox(bounds, GRAY);
}

void ModelComponent::Start() {
}

void ModelComponent::Update() {
    if (model == nullptr) return;
    bounds = GetModelBoundingBox(*model);
}

void ModelComponent::LoadModelFromFile(const std::string &filename) {
    if (filename.empty()) {
        warningText = "Filename is empty";
        std::cout << warningText << std::endl;
        return;
    }
    // test file exits
    if (!FileExists(filename.c_str())) {
        warningText = "File does not exist";
        std::cout << warningText << std::endl;
        return;
    }
    warningText = "";
    if (IsFileExtension(filename.c_str(), ".obj") ||
        IsFileExtension(filename.c_str(), ".gltf") ||
        IsFileExtension(filename.c_str(), ".glb") ||
        IsFileExtension(filename.c_str(), ".vox") ||
        IsFileExtension(filename.c_str(), ".iqm") ||
        IsFileExtension(filename.c_str(), ".m3d")) // Model file formats supported
    {
        if (model) UnloadModel(*model); // Unload previous model
        const Model modelObj = LoadModel(filename.c_str()); // Load new model
        model = new Model(modelObj);
        bounds = GetModelBoundingBox(*model);
    }
}
