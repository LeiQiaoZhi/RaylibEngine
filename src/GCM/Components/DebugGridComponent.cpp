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

void DebugGridComponent::OnDraw(Scene *scene) const {
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

void DebugGridComponent::Start() {
}

void DebugGridComponent::Update() {
}

void DebugGridComponent::OnDrawGizmosBottom(Scene *scene) const {
}


