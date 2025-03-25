#include "RigidbodyComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"
#include "../../editor/LuaProperty.h"

RigidbodyComponent::RigidbodyComponent() {
    headerProperty.label = "Rigidbody";
}

void RigidbodyComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) {
        height = rect.y - originalY;
        return;
    }

    GuiToggleGroup({rect.x, rect.y, rect.width / 3.0f, Editor::TextSize() * 1.5f},
                   "Static;Kinematic;Dynamic", reinterpret_cast<int *>(&rigidbodyType));
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    massProperty.OnEditorGUI(rect);
    dragProperty.OnEditorGUI(rect);

    // Status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded()) {
        Editor::BeginIndent(rect, Editor::LargeGap());
        std::ostringstream oss;
        oss.precision(2);
        oss << "Velocity: [" << velocity.x << ", " << velocity.y << ", " << velocity.z << "]" << std::endl;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();

        oss.str("");
        oss << "Acceleration: [" << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << "]" << std::endl;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();

        Editor::EndIndent(rect, Editor::LargeGap());
    }

    height = rect.y - originalY;
}

float RigidbodyComponent::GetEditorHeight() const {
    return height;
}

void RigidbodyComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void RigidbodyComponent::OnEditorDraw(Scene *scene) const {
}

void RigidbodyComponent::EditorStart() {
}

void RigidbodyComponent::OnDrawGizmos(Scene *scene) const {
}

void RigidbodyComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void RigidbodyComponent::EditorUpdate() {
}

void RigidbodyComponent::Update() {
}

nlohmann::json RigidbodyComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "RigidbodyComponent";
    return j;
}

void RigidbodyComponent::FromJson(const nlohmann::json &json) {
}
