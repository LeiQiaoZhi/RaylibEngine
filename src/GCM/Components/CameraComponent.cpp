#include "CameraComponent.h"
#include "raylib.h"
#include "../../editor/Editor.h"

void CameraComponent::OnEditorGUI(Rectangle &rect) {
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
}

float CameraComponent::GetEditorHeight() const {
    return headerProperty.IsFolded()
               ? Editor::TextSize() + Editor::SmallGap()
               : Editor::TextSize() * 5 + Editor::SmallGap() * 3;
}

void CameraComponent::OnDraw(Scene *scene) const {
}

void CameraComponent::OnDrawGizmos(Scene *scene) const {
    // TODO: visualize frustum
}

void CameraComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void CameraComponent::Start() {
}

void CameraComponent::Update() {
    if (IsCursorHidden())
        UpdateCamera(camera, cameraMode);
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (IsCursorHidden()) {
            EnableCursor();
        } else {
            DisableCursor();
        }
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
}
