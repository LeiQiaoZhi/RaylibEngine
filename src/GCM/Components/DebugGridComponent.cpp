#include "DebugGridComponent.h"

#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"

void DebugGridComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    sizeProperty.OnEditorGUI(rect);
    spacingProperty.OnEditorGUI(rect);
    normalProperty.OnEditorGUI(rect);
    colorProperty.OnEditorGUI(rect);

    editorHeight = rect.y - originalY;
}

float DebugGridComponent::GetEditorHeight() const {
    return editorHeight;
}

void DebugGridComponent::OnDraw(Scene *scene) const {
}

void DebugGridComponent::OnDrawGizmos(Scene *scene) const {
    DrawDebugGrid();
}

void DebugGridComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void DebugGridComponent::Start() {
}

void DebugGridComponent::Update() {
}

void DebugGridComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void DebugGridComponent::DrawDebugGrid() const {
    const TransformComponent *transform = gameObject->GetTransform();
    const Vector3 position = transform->GetWorldPosition();
    const Vector3 right = transform->GetWorldRight();
    const Vector3 forward = transform->GetWorldForward();
    Vector3 bottomLeft = position - right * size.x / 2 - forward * size.y / 2;

    for (int i = 0; i <= size.x / spacing.x; i++) {
        const Vector3 start = bottomLeft + right * i * spacing.x;
        const Vector3 end = start + forward * size.y;
        DrawLine3D(start, end, color);
    }
    for (int i = 0; i <= size.y / spacing.y; i++) {
        const Vector3 start = bottomLeft + forward * i * spacing.y;
        const Vector3 end = start + right * size.x;
        DrawLine3D(start, end, color);
    }
}
