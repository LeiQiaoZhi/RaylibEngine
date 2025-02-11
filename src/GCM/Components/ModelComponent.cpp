#include "ModelComponent.h"

#include <config.h>
#include <iostream>
#include <ostream>
#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "../../editor/Editor.h"
#include "../../GCM/GameObject.h"

void ModelComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    // file name
    const float labelWidth = MeasureText("Filename", Editor::TextSize());
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "Filename");
    rect.x += labelWidth + Editor::SmallGap();
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width - Editor::SmallGap() - labelWidth, Editor::TextSize() * 1.0f},
                   filename, 32, editMode)) {
        editMode = !editMode;
    }
    rect.x -= labelWidth + Editor::SmallGap();
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // load model
    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load Model")) {
        if (filename[0] == '\0') {
            warningText = "Filename is empty";
            return;
        }
        const std::string path = std::string(ASSET_DIR) + "/models/" + filename;
        LoadModelFromFile(path);
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // warning
    if (!warningText.empty()) {
        const char *text = GuiIconText(ICON_WARNING, warningText.c_str());
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, text);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded() && model != nullptr) {
        Editor::BeginIndent(rect, Editor::LargeGap());

        // Draw options
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Surface", &drawSurface);
        rect.x += rect.width * .33f;
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Wireframe",
                    &drawWireframe);
        rect.x += rect.width * .33f;
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Bounds", &drawBounds);
        rect.x -= rect.width * .66f;
        rect.y += Editor::TextSize() + Editor::SmallGap();

        // Mesh
        std::ostringstream oss;
        oss.precision(2);
        oss << "Mesh count: " << model->meshCount << std::endl;
        rect.y += Editor::TextSize();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
        Editor::BeginIndent(rect, Editor::LargeGap());
        for (int i = 0; i < model->meshCount; i++) {
            const Mesh mesh = model->meshes[i];
            int materialIndex = model->meshMaterial[i];
            oss.str("");
            oss << i << ". " << mesh.vertexCount << " vertices, " << mesh.triangleCount << " triangles" <<
                    ", material: [" << materialIndex << "]" << std::endl;
            GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
            rect.y += Editor::TextSize() + Editor::SmallGap();
        }
        Editor::EndIndent(rect, Editor::LargeGap());

        // Material
        oss.str("");
        oss << "Material count: " << model->materialCount << std::endl;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
        Editor::BeginIndent(rect, Editor::LargeGap());
        for (int i = 0; i < model->materialCount; i++) {
            const Material material = model->materials[i];
            oss.str("");
            oss << i << ". " << "Shader ID: [" << material.shader.id << "], Maps: " << std::endl;
            GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
            rect.y += Editor::TextSize() + Editor::SmallGap();
            for (int i = 0; i < MAX_MATERIAL_MAPS; i++) {
                std::string mapType = RaylibUtils::MaterialMapTypeToString(i);
                if (material.maps[i].texture.id != 0) {
                    oss.str("");
                    oss << "   [" << i << ", " << mapType << ", Texture" << material.maps[i].texture.id << "]" <<
                            std::endl;
                    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
                    rect.y += Editor::TextSize() + Editor::SmallGap();
                }
            }
        }
        Editor::EndIndent(rect, Editor::LargeGap());

        // Material Loading

        Editor::EndIndent(rect, Editor::LargeGap());
    }
    height = rect.y - originalY;
}

float ModelComponent::GetEditorHeight() const {
    return height;
}

void ModelComponent::OnDraw(Scene *scene) const {
    if (model == nullptr) return;

    if (drawSurface)
        DrawModel(*model, {0, 0, 0}, 1.0f, WHITE);

    if (drawWireframe)
        DrawModelWires(*model, {0, 0, 0}, 1.0f, WHITE);
}

void ModelComponent::OnDrawGizmos(Scene *scene) const {
}

void ModelComponent::OnDrawGizmosSelected(Scene *scene) const {
    if (drawBounds && model != nullptr)
        RaylibUtils::DrawModelBoundingBoxAfterTransform(*model, GRAY);
}

void ModelComponent::Start() {
}

void ModelComponent::Update() {
    if (model == nullptr) return;
    model->transform = gameObject->GetTransform()->GetTransformMatrix();
}

void ModelComponent::LoadModelFromFile(const std::string &filename) {
    // test file exits
    if (!FileExists(filename.c_str())) {
        warningText = "File does not exist";
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
    } else {
        warningText = "File format not supported";
    }
}

void ModelComponent::OnDrawGizmosBottom(Scene *scene) const {
}
