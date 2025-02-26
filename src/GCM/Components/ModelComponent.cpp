#include "ModelComponent.h"

#include <config.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/stat.h>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
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
            LoadModelFromFile(filename);
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

        // Picking
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Picking", &isPicking);
        rect.x += rect.width * .33f;
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Bones Lines",
                    &drawBonesLines);
        rect.x += rect.width * .33f;
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Bone Names",
                    &drawBoneNames);
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
            const bool highlighed = highlightedMeshIndex == i;
            DrawRectangleRec({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                             highlighed ? Editor::DisabledColor() : Editor::BackgroundColor());
            const Mesh mesh = model->meshes[i];
            int materialIndex = model->meshMaterial[i];
            const float buttonWidth = Editor::TextWidth("Highlight") + Editor::LargeGap();
            if (GuiButton({rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.0f},
                          highlighed ? "Cancel" : "Highlight")) {
                if (highlighed)
                    SetHighlightedMesh(-1);
                else
                    SetHighlightedMesh(i);
            }
            Editor::BeginIndent(rect, buttonWidth);
            oss.str("");
            oss << i << ". " << mesh.vertexCount << " vertices, " << mesh.triangleCount << " triangles" <<
                    ", material: [" << materialIndex << "]" << std::endl;
            rect.y += Editor::TextSize() * 0.5f + Editor::SmallGap();
            GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
            rect.y += Editor::TextSize() * 0.5f + Editor::SmallGap();
            Editor::EndIndent(rect, buttonWidth);
        }
        Editor::EndIndent(rect, Editor::LargeGap());
        rect.y += Editor::TextSize() + Editor::SmallGap();

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
            // lights
            scene->SendLightInfoToModel(model);
            // custom params
            for (auto &materialProp: materialProps)
                materialProp.SendParamsToShader();

            DrawModel(*model, {0, 0, 0}, 1.0f, WHITE);

            if (highlightedMeshIndex != -1)
                DrawMesh(model->meshes[highlightedMeshIndex], highlightedMaterial, model->transform);
        } else if (renderType == RenderType::DebugUV)
            RaylibUtils::DrawModelWithShader(*model, {0, 0, 0}, 1.0f, WHITE, RaylibUtils::GetUVShader());
        else if (renderType == RenderType::DebugNormal)
            RaylibUtils::DrawModelWithShader(*model, {0, 0, 0}, 1.0f, WHITE, RaylibUtils::GetNormalShader());
    }

    if (drawWireframe)
        DrawModelWires(*model, {0, 0, 0}, 1.0f, WHITE);
}

void ModelComponent::OnDrawGizmos(Scene *scene) const {
    if (model == nullptr) return;
}

void ModelComponent::OnDrawGizmosSelected(Scene *scene) const {
    if (model == nullptr) return;
    const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
    const Vector3 worldPosition = gameObject->GetTransform()->GetWorldPosition();
    const float distToCamera = Vector3Distance(worldPosition, camera->position);
    const float radius = distToCamera * 0.002f;

    if (drawBounds)
        RaylibUtils::DrawModelBoundingBoxAfterTransform(*model, GRAY);

    // show picking information
    if (isPicking && highlightedMeshIndex != -1) {
        EndMode3D();
        rlDisableDepthTest();

        const Mesh *mesh = &model->meshes[highlightedMeshIndex];
        const Vector3 worldPosition = Vector3Transform({
                                                           mesh->vertices[0], mesh->vertices[1], mesh->vertices[2]
                                                       }, model->transform);
        const float padding = 5;
        const Vector2 screenPosition = GetWorldToScreenEx(worldPosition, *camera, scene->screenSpaceRect.width,
                                                          scene->screenSpaceRect.height);
        std::ostringstream oss;
        oss << "Mesh: [" << std::to_string(highlightedMeshIndex)
                << "], Material: [" << std::to_string(model->meshMaterial[highlightedMeshIndex]) << "]" << std::endl;
        const float width = MeasureText(oss.str().c_str(), Editor::TextSize());
        DrawRectangle(screenPosition.x - padding, screenPosition.y - padding,
                      width + padding * 2, Editor::TextSize() + padding * 2, BLACK);
        DrawText(oss.str().c_str(), screenPosition.x, screenPosition.y, Editor::TextSize(), WHITE);

        BeginMode3D(*camera);
        rlEnableDepthTest();
    }

    // bones
    if (drawBonesLines) {
        rlDisableDepthTest();
        for (int i = 0; i < model->boneCount; i++) {
            const BoneInfo bone = model->bones[i];
            const Transform boneTransform = model->bindPose[i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);
            if (bone.parent >= 0 && bone.parent < model->boneCount) {
                const Transform parentBoneTransform = model->bindPose[bone.parent];
                const Vector3 parentBonePosition = Vector3Transform(parentBoneTransform.translation, model->transform);
                DrawLine3D(bonePosition, parentBonePosition, GREEN);
            }
            // DrawSphere(bonePosition, radius, GREEN);
        }
    }

    if (drawBoneNames) {
        EndMode3D();
        for (int i = 0; i < model->boneCount; i++) {
            const BoneInfo bone = model->bones[i];
            const Transform boneTransform = model->bindPose[i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);

            const float padding = 5;
            const float width = MeasureText(bone.name, 10);
            Vector2 screenPosition = GetWorldToScreenEx(bonePosition, *camera, scene->screenSpaceRect.width,
                                                        scene->screenSpaceRect.height);
            screenPosition += Vector2{radius, -radius}; // offset a bit
            DrawRectangle(screenPosition.x - padding, screenPosition.y - padding,
                          width + padding * 2, 10 + padding * 2, Fade(BLACK, 0.5f));
            DrawText(bone.name, screenPosition.x, screenPosition.y, 10, WHITE);
        }
        BeginMode3D(*camera);
        rlEnableDepthTest();
    }

    // bones -- when playing animation
    ModelAnimation *animation = &animations[0];
    if (drawBonesLines) {
        rlDisableDepthTest();
        for (int i = 0; i < animation->boneCount; i++) {
            const BoneInfo bone = animation->bones[i];
            const Transform boneTransform = animation->framePoses[animationFrameCounter][i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);
            if (bone.parent >= 0 && bone.parent < model->boneCount) {
                const Transform parentBoneTransform = animation->framePoses[animationFrameCounter][bone.parent];
                const Vector3 parentBonePosition = Vector3Transform(parentBoneTransform.translation, model->transform);
                DrawLine3D(bonePosition, parentBonePosition, ORANGE);
            }
            // DrawSphere(bonePosition, radius, ORANGE);
        }
    }

    if (drawBoneNames) {
        EndMode3D();
        for (int i = 0; i < animation->boneCount; i++) {
            const BoneInfo bone = animation->bones[i];
            const Transform boneTransform = animation->framePoses[animationFrameCounter][i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);

            const float padding = 5;
            const float width = MeasureText(bone.name, 10);
            Vector2 screenPosition = GetWorldToScreenEx(bonePosition, *camera, scene->screenSpaceRect.width,
                                                        scene->screenSpaceRect.height);
            screenPosition += Vector2{radius, -radius}; // offset a bit
            DrawRectangle(screenPosition.x - padding, screenPosition.y - padding,
                          width + padding * 2, 10 + padding * 2, Fade(BLACK, 0.5f));
            DrawText(bone.name, screenPosition.x, screenPosition.y, 10, WHITE);
        }
        BeginMode3D(*camera);
        rlEnableDepthTest();
    }
}

void ModelComponent::Start() {
}

void ModelComponent::Update() {
    if (model == nullptr) return;
    model->transform = gameObject->GetTransform()->GetTransformMatrix();

    if (isPicking && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        const Ray ray = gameObject->scene->GetMouseScreenToWorldRay();
        int selectedMeshIndex = -1;
        RayCollision selection = RaylibUtils::GetRayCollisionModel(ray, *model, &selectedMeshIndex);
        std::cout << "Selection: " << selectedMeshIndex << std::endl;
        SetHighlightedMesh(selectedMeshIndex);
    }

    if (animationCount > 0) {
        animationFrameCounter = (animationFrameCounter + 1) % animations[0].frameCount;
        UpdateModelAnimation(*model, animations[0], animationFrameCounter);
    }
}

void ModelComponent::LoadModelFromFile(const std::string &filename) {
    const std::string path = ASSET_DIR + std::string("/models/") + filename;
    // test file exits
    if (!FileExists(path.c_str())) {
        statusText = "File does not exist";
        statusWarning = true;
        return;
    }
    statusText = "";
    if (IsFileExtension(path.c_str(), ".obj") ||
        IsFileExtension(path.c_str(), ".gltf") ||
        IsFileExtension(path.c_str(), ".glb") ||
        IsFileExtension(path.c_str(), ".vox") ||
        IsFileExtension(path.c_str(), ".iqm") ||
        IsFileExtension(path.c_str(), ".m3d")) // Model file formats supported
    {
        if (model) UnloadModel(*model); // Unload previous model
        const Model modelObj = LoadModel(path.c_str()); // Load new model
        model = new Model(modelObj);

        // update properties
        materialProps.clear();
        for (int i = 0; i < model->meshCount; i++)
            materialProps.emplace_back(model, i);

        // load animation
        if (!IsFileExtension(path.c_str(), ".obj") && !IsFileExtension(path.c_str(), ".vox")) {
            animations = LoadModelAnimations(path.c_str(), &animationCount);
        }

        statusText = "Model loaded";
        statusWarning = false;
    } else {
        statusText = "File format not supported";
        statusWarning = true;
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

nlohmann::json ModelComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "ModelComponent";
    j["filename"] = filename;
    j["drawSurface"] = drawSurface;
    j["drawBounds"] = drawBounds;
    j["drawWireframe"] = drawWireframe;
    j["drawBonesLines"] = drawBonesLines;
    j["drawBoneNames"] = drawBoneNames;
    j["renderType"] = static_cast<int>(renderType);
    j["isPicking"] = isPicking;

    std::vector<nlohmann::json> materialPropsJson;
    for (const auto &materialProp: materialProps)
        materialPropsJson.emplace_back(materialProp.ToJson());
    j["materials"] = materialPropsJson;
    return j;
}

void ModelComponent::FromJson(const nlohmann::json &json) {
    std::strncpy(filename, json.at("filename").get<std::string>().c_str(), 32);
    drawSurface = json.value("drawSurface", drawSurface);
    drawBounds = json.value("drawBounds", drawBounds);
    drawWireframe = json.value("drawWireframe", drawWireframe);
    drawBonesLines = json.value("drawBonesLines", drawBonesLines);
    drawBoneNames = json.value("drawBoneNames", drawBoneNames);
    renderType = static_cast<RenderType>(json.at("renderType").get<int>());
    isPicking = json.value("isPicking", isPicking);

    LoadModelFromFile(filename);

    int i = 0;
    for (const auto &materialPropJson: json.at("materials")) {
        if (i >= materialProps.size()) {
            statusText = "More materials than expected";
            statusWarning = true;
            break;
        }
        materialProps[i].FromJson(materialPropJson);
        i++;
    }
}

void ModelComponent::SetHighlightedMesh(int index) {
    // reset material
    if (highlightedMeshIndex != -1)
        model->materials[model->meshMaterial[highlightedMeshIndex]].shader = originalShader;

    highlightedMeshIndex = index;

    // set material
    if (index != -1) {
        highlightedMaterial = model->materials[model->meshMaterial[highlightedMeshIndex]];
        originalShader = highlightedMaterial.shader;
        highlightedMaterial.shader = highlightedShader;
    }
}
