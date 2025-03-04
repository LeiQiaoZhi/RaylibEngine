#include "SphereColliderComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"
#include "../../editor/LuaProperty.h"

SphereColliderComponent::SphereColliderComponent() {
    headerProperty.label = "Sphere Collider";
}

void SphereColliderComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) {
        height = rect.y - originalY;
        return;
    }

   radiusProperty.OnEditorGUI(rect);

    // Status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    height = rect.y - originalY;
}

float SphereColliderComponent::GetEditorHeight() const {
    return height;
}

void SphereColliderComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void SphereColliderComponent::OnEditorDraw(Scene *scene) const {
}

void SphereColliderComponent::EditorStart() {
}

void SphereColliderComponent::OnDrawGizmos(Scene *scene) const {
    DrawSphereWires(gameObject->GetTransform()->GetWorldPosition(), radius, 16, 16, GREEN);
}

void SphereColliderComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void SphereColliderComponent::EditorUpdate() {
}

void SphereColliderComponent::Update() {
}

nlohmann::json SphereColliderComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "SphereColliderComponent";
    j["radius"] = radius;
    return j;
}

void SphereColliderComponent::FromJson(const nlohmann::json &json) {
    radius = json.value("radius", radius);
}
