#include "LightComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"

void LightComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    int *lightTypePtr = reinterpret_cast<int *>(&lightType);
    GuiToggleGroup({rect.x, rect.y, rect.width / 3, Editor::TextSize() * 1.5f}, "Ambient;Directional;Point",
                   lightTypePtr);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    intensityProperty.OnEditorGUI(rect);

    if (lightType == LightType::Point)
        rangeProperty.OnEditorGUI(rect);

    colorProperty.OnEditorGUI(rect);

    height = rect.y - originalY;
}

float LightComponent::GetEditorHeight() const {
    return height;
}

void LightComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void LightComponent::OnDraw(Scene *scene) const {
}

void LightComponent::Start() {
}

void LightComponent::OnDrawGizmos(Scene *scene) const {
}

void LightComponent::OnDrawGizmosSelected(Scene *scene) const {
    if (lightType == LightType::Directional) {
        const Vector3 direction = gameObject->GetTransform()->GetWorldForward();
        const Vector3 position = gameObject->GetTransform()->GetWorldPosition();
        const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
        const float distToCamera = Vector3Distance(position, camera->position);
        DrawCylinderEx(position, position + direction * distToCamera * 0.1f,
                       distToCamera * 0.01, distToCamera * 0.01, 10, color);
        DrawCylinderEx(position + direction * distToCamera * 0.1f, position + direction * distToCamera * 0.15f,
                       distToCamera * 0.02, distToCamera * 0.0, 10, color);
    } else if (lightType == LightType::Point) {
        const Vector3 position = gameObject->GetTransform()->GetWorldPosition();
        DrawSphereWires(position, range, 8, 8, color);
    }
}

void LightComponent::Update() {
}
