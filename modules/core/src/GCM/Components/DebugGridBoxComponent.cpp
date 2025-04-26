#include "DebugGridBoxComponent.h"

#include "rlgl.h"
#include "../GameObject.h"

void DebugGridBoxComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    sizeProperty.OnEditorGUI(rect);
    spacingProperty.OnEditorGUI(rect);
    colorProperty.OnEditorGUI(rect);

    editorHeight = rect.y - originalY;
}

float DebugGridBoxComponent::GetEditorHeight() const {
    return editorHeight;
}

void DebugGridBoxComponent::OnEditorDraw(Scene *scene) const {
}

void DebugGridBoxComponent::OnDrawGizmos(Scene *scene) const {
    const TransformComponent *transform = gameObject->GetTransform();
    const Vector3 position = transform->GetWorldPosition();
    const Vector3 right = transform->GetWorldRight();
    const Vector3 up = transform->GetWorldUp();
    const Vector3 forward = transform->GetWorldForward();
    for (int i = -1; i <= 1; i += 2) {
        // normal is up
        Vector3 gridPos = position + up * i * size.y / 2;
        Vector2 gridSize = {size.x, size.z};
        Vector2 gridSpacing = {spacing.x, spacing.z};
        RaylibUtils::DrawDebugGrid(gridPos, right, forward, gridSize, gridSpacing, color);

        // normal is right
        gridPos = position + right * i * size.x / 2;
        gridSize = {size.y, size.z};
        gridSpacing = {spacing.y, spacing.z};
        RaylibUtils::DrawDebugGrid(gridPos, up, forward, gridSize, gridSpacing, color);

        // normal is forward
        gridPos = position + forward * i * size.z / 2;
        gridSize = {size.x, size.y};
        gridSpacing = {spacing.x, spacing.y};
        RaylibUtils::DrawDebugGrid(gridPos, right, up, gridSize, gridSpacing, color);
    }
}

void DebugGridBoxComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void DebugGridBoxComponent::EditorStart() {
}

void DebugGridBoxComponent::EditorUpdate() {
}

void DebugGridBoxComponent::OnDrawGizmosBottom(Scene *scene) const {
}

nlohmann::json DebugGridBoxComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "DebugGridBoxComponent";
    j["size"] = {size.x, size.y, size.z};
    j["spacing"] = {spacing.x, spacing.y, spacing.z};
    j["color"] = {color.r, color.g, color.b, color.a};
    return j;
}

void DebugGridBoxComponent::FromJson(const nlohmann::json &json) {
    size = {json["size"][0], json["size"][1], json["size"][2]};
    spacing = {json["spacing"][0], json["spacing"][1], json["spacing"][2]};
    color = {json["color"][0], json["color"][1], json["color"][2], json["color"][3]};
    colorProperty.UpdateColor(color);
}
