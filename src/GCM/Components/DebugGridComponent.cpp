#include "DebugGridComponent.h"

#include "rlgl.h"
#include "../GameObject.h"

void DebugGridComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    sizeProperty.OnEditorGUI(rect);
    spacingProperty.OnEditorGUI(rect);
    colorProperty.OnEditorGUI(rect);

    editorHeight = rect.y - originalY;
}

float DebugGridComponent::GetEditorHeight() const {
    return editorHeight;
}

void DebugGridComponent::OnEditorDraw(Scene *scene) const {
}

void DebugGridComponent::OnDrawGizmos(Scene *scene) const {
    const TransformComponent *transform = gameObject->GetTransform();
    const Vector3 position = transform->GetWorldPosition();
    const Vector3 right = transform->GetWorldRight();
    const Vector3 forward = transform->GetWorldForward();
    RaylibUtils::DrawDebugGrid(position, right, forward, size, spacing, color);
}

void DebugGridComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void DebugGridComponent::EditorStart() {
}

void DebugGridComponent::EditorUpdate() {
}

void DebugGridComponent::OnDrawGizmosBottom(Scene *scene) const {
}

nlohmann::json DebugGridComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "DebugGridComponent";
    j["size"] = {size.x, size.y};
    j["spacing"] = {spacing.x, spacing.y};
    j["color"] = {color.r, color.g, color.b, color.a};
    return j;
}

void DebugGridComponent::FromJson(const nlohmann::json &json) {
    size = {json["size"][0], json["size"][1]};
    spacing = {json["spacing"][0], json["spacing"][1]};
    color = {json["color"][0], json["color"][1], json["color"][2], json["color"][3]};
    colorProperty.UpdateColor(color);
}


