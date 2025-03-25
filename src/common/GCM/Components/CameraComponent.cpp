#include "CameraComponent.h"
#include "raylib.h"
#include "rlgl.h"
#include "../Scene.h"
#include "../../editor/Editor.h"

void CameraComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    const Rectangle modeRect = {rect.x, rect.y, rect.width / 5.0f, Editor::TextSize() * 2.0f};
    GuiToggleGroup(modeRect, "Custom;Free;Orbital;FPS;TP", &cameraMode);
    rect.y += Editor::TextSize() * 2 + Editor::SmallGap();

    Rectangle fovRect = {rect.x, rect.y, rect.width, Editor::TextSize() * 2.0f};
    GuiLabel(fovRect, "FOV");
    fovRect.x += Editor::TextSize() * 4.0;
    fovRect.width -= Editor::TextSize() * 4.0;
    GuiSlider(fovRect, "", "", &camera->fovy, 1, 179.0);
    rect.y += Editor::TextSize() * 2 + Editor::SmallGap();

    int *backgroundModePtr = reinterpret_cast<int *>(&backgroundMode);
    GuiToggleGroup({rect.x, rect.y, rect.width / 2.0f, Editor::TextSize() * 1.5f},
                   "Color;Skybox", backgroundModePtr);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    if (backgroundMode == BackgroundMode::Color) {
        backgroundColorProperty.OnEditorGUI(rect);
    } else {
        GuiCheckBox({rect.x, rect.y, Editor::TextSize() * 1.0f, Editor::TextSize() * 1.0f}, "Flip Y", &skyboxFlipY);
        rect.y += Editor::TextSize() + Editor::SmallGap();

        skyboxMaterialProperty.OnEditorGUI(rect);
    }

    height = rect.y - originalY;
}

float CameraComponent::GetEditorHeight() const {
    return height;
}

void CameraComponent::OnEditorDraw(Scene *scene) const {
}

void CameraComponent::OnDrawGizmos(Scene *scene) const {
    // TODO: visualize frustum
}

void CameraComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void CameraComponent::EditorStart() {
}

void CameraComponent::EditorUpdate() {
    if (!gameObject->scene->IsInsideScenePanel(GetMousePosition())) return;
    camera->position = gameObject->GetTransform()->GetWorldPosition();
    if (updating) {
        UpdateCamera(camera, cameraMode);
        gameObject->GetTransform()->SetWorldPosition(camera->position);
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        updating = !updating;
    }
}

void CameraComponent::OnDrawGizmosBottom(Scene *scene) const {
}

nlohmann::json CameraComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "CameraComponent";
    j["cameraMode"] = cameraMode;
    // TODO: save camera transform
    j["cameraPosition"] = {camera->position.x, camera->position.y, camera->position.z};
    j["cameraTarget"] = {camera->target.x, camera->target.y, camera->target.z};
    j["cameraUp"] = {camera->up.x, camera->up.y, camera->up.z};
    j["cameraFovy"] = camera->fovy;
    j["backgroundMode"] = backgroundMode;
    j["backgroundColor"] = {backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a};
    j["skyboxFlipY"] = skyboxFlipY;
    j["skyboxMaterial"] = skyboxMaterialProperty.ToJson();
    return j;
}

void CameraComponent::FromJson(const nlohmann::json &json) {
    cameraMode = json["cameraMode"];
    if (json.contains("cameraPosition"))
        camera->position = {json["cameraPosition"][0], json["cameraPosition"][1], json["cameraPosition"][2]};
    if (json.contains("cameraTarget"))
        camera->target = {json["cameraTarget"][0], json["cameraTarget"][1], json["cameraTarget"][2]};
    if (json.contains("cameraUp"))
        camera->up = {json["cameraUp"][0], json["cameraUp"][1], json["cameraUp"][2]};
    camera->fovy = json.value("cameraFovy", 45.0f);
    backgroundMode = static_cast<BackgroundMode>(json.value("backgroundMode", backgroundMode));
    if (json.contains("backgroundColor"))
        backgroundColor = {
            json["backgroundColor"][0], json["backgroundColor"][1], json["backgroundColor"][2],
            json["backgroundColor"][3]
        };
    skyboxFlipY = json.value("skyboxFlipY", skyboxFlipY);
    if (json.contains("skyboxMaterial"))
        skyboxMaterialProperty.FromJson(json["skyboxMaterial"]);
}

void CameraComponent::DrawBackground() {
    if (backgroundMode == BackgroundMode::Color) {
        ClearBackground(backgroundColor);
        return;
    }

    // Draw skybox
    if (skybox.meshCount == 0) return;
    rlDisableDepthMask();
    rlDisableBackfaceCulling();
    Material *material = &skybox.materials[0];
    SetShaderValue(material->shader, GetShaderLocation(material->shader, "flipY"), &skyboxFlipY, SHADER_UNIFORM_INT);
    SetShaderValue(material->shader, GetShaderLocation(material->shader, "viewPos"),
                   &camera->position, SHADER_UNIFORM_VEC3);
    DrawModel(skybox, camera->position, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}
