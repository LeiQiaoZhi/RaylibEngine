#include "ModelComponent.h"

#include <config.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/stat.h>

#include "raylib.h"
#include "raymath.h"
#include "../../editor/Editor.h"
#include "../../GCM/GameObject.h"

void ModelComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    // file name
    const float labelWidth = Editor::TextWidth("Filename") + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "Filename");
    rect.x += labelWidth;
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width - Editor::SmallGap() - labelWidth, Editor::TextSize() * 1.0f},
                   filename, 32, editMode)) {
        editMode = !editMode;
    }
    rect.x -= labelWidth;
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // load model
    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load Model")) {
        if (filename[0] == '\0') {
            statusText = "Filename is empty";
            statusWarning = true;
        } else {
            const std::string path = std::string(ASSET_DIR) + "/models/" + filename;
            LoadModelFromFile(path);
            statusText = "Model loaded";
            statusWarning = false;
        }
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    // Material Loading
    materialsFoldout.SetLabel(TextFormat("Materials [%d]", materialProps.size()));
    materialsFoldout.OnEditorGUI(rect);
    if (!materialsFoldout.IsFolded()) {
        Editor::BeginIndent(rect, Editor::LargeGap());
        for (auto &materialProp: materialProps)
            materialProp.OnEditorGUI(rect);
        Editor::EndIndent(rect, Editor::LargeGap());
    }

    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded() && model != nullptr) {
        Editor::BeginIndent(rect, Editor::LargeGap());

        // Render types
        auto renderTypePtr = reinterpret_cast<int *>(&renderType);
        GuiToggleGroup({rect.x, rect.y, rect.width / 3, Editor::TextSize() * 1.5f}, "Default;UV;Normal", renderTypePtr);
        rect.y += Editor::TextSize() * 1.5f + Editor::MediumGap();

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
                    float recWidth = Editor::TextSize() * 1.0f;
                    DrawRectangleRec({
                                         rect.x + rect.width - recWidth, rect.y - recWidth * 1.0f, recWidth,
                                         Editor::TextSize() * 1.0f
                                     }, material.maps[i].color);
                    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
                    rect.y += Editor::TextSize() + Editor::SmallGap();
                }
            }
        }
        Editor::EndIndent(rect, Editor::LargeGap());
        Editor::EndIndent(rect, Editor::LargeGap());
    }

    height = rect.y - originalY;
}

float ModelComponent::GetEditorHeight() const {
    return height;
}

void ModelComponent::OnDraw(Scene *scene) const {
    if (model == nullptr) return;

    if (drawSurface) {
        if (renderType == RenderType::Default) {
            scene->SendLightInfoToModel(model);
            DrawModel(*model, {0, 0, 0}, 1.0f, WHITE);
        }
        else if (renderType == RenderType::DebugUV)
            RaylibUtils::DrawModelWithShader(*model, {0, 0, 0}, 1.0f, WHITE, RaylibUtils::GetUVShader());
        else if (renderType == RenderType::DebugNormal)
            RaylibUtils::DrawModelWithShader(*model, {0, 0, 0}, 1.0f, WHITE, RaylibUtils::GetNormalShader());
    }

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
        statusText = "File does not exist";
        return;
    }
    statusText = "";
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

        // update properties
        materialProps.clear();
        for (int i = 0; i < model->meshCount; i++)
            materialProps.emplace_back(model, i);
    } else {
        statusText = "File format not supported";
    }
}

void ModelComponent::SetModelFromMesh(const Mesh &mesh) {
    const Model modelObj = LoadModelFromMesh(mesh);
    model = new Model(modelObj);

    // update properties
    materialProps.clear();
    for (int i = 0; i < model->meshCount; i++)
        materialProps.emplace_back(model, i);
}

void ModelComponent::OnDrawGizmosBottom(Scene *scene) const {
}
