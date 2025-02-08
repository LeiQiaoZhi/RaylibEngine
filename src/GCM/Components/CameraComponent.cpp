#include "CameraComponent.h"
#include "raylib.h"
#include "../../editor/Editor.h"

void CameraComponent::OnEditorGUI(Rectangle &rect) {
    DrawText("Component: Camera", rect.x, rect.y, Editor::TextSize(), Editor::TextColor());
    rect.y += Editor::TextSize() + Editor::SmallGap();

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
    return Editor::TextSize() * 5 + Editor::SmallGap() * 3;
}

void CameraComponent::OnDrawGizmos(Scene* scene) const{
    // TODO: visualize frustum
}

void CameraComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void CameraComponent::Update() {
    UpdateCamera(camera, cameraMode);
}
