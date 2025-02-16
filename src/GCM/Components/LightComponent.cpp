#include "LightComponent.h"

#include <sstream>
#include <external/glad.h>

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

nlohmann::json LightComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "LightComponent";
    j["lightType"] = static_cast<int>(lightType);
    j["intensity"] = intensity;
    j["range"] = range;
    j["color"] = {color.r, color.g, color.b, color.a};
    return j;
}

void LightComponent::FromJson(const nlohmann::json &json) {
    lightType = static_cast<LightType>(json.at("lightType").get<int>());
    intensity = json.at("intensity").get<float>();
    range = json.at("range").get<float>();
    color = {json["color"][0], json["color"][1], json["color"][2], json["color"][3]};
    colorProperty.UpdateColor(color);
}

void LightComponent::OnDraw(Scene *scene) const {
}

void LightComponent::Start() {
}

void LightComponent::OnDrawGizmos(Scene *scene) const {
}

void LightComponent::OnDrawGizmosSelected(Scene *scene) const {
    if (lightType == LightType::Directional) {
        rlDisableDepthTest();
        const Vector3 direction = gameObject->GetTransform()->GetWorldForward();
        const Vector3 position = gameObject->GetTransform()->GetWorldPosition();
        const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
        const float distToCamera = Vector3Distance(position, camera->position);
        const Vector3 rotationAxis = Vector3CrossProduct({0, 1, 0}, direction);
        float rotationAngle = atan2f(Vector3Length(rotationAxis), Vector3DotProduct({0, 1, 0}, direction));
        rotationAngle *= RAD2DEG;
        const Vector3 scale = {distToCamera * 0.2f, distToCamera * 0.2f, distToCamera * 0.2f};
        DrawModelEx(arrowModel, position, rotationAxis, rotationAngle, scale, color);
        rlEnableDepthTest();
    } else if (lightType == LightType::Point) {
        const Vector3 position = gameObject->GetTransform()->GetWorldPosition();
        DrawSphereWires(position, range, 8, 8, color);
    }
}

void LightComponent::Update() {
}
