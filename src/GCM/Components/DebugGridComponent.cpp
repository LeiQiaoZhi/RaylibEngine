#include "DebugGridComponent.h"

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
}

void DebugGridComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void DebugGridComponent::Start() {
}

void DebugGridComponent::Update() {
}
