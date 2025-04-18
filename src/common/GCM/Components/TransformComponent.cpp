#include "TransformComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"

void TransformComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    positionProperty.OnEditorGUI(rect);
    eulerAnglesProperty.OnEditorGUI(rect);
    scaleProperty.OnEditorGUI(rect);

    debugFoldout.OnEditorGUI(rect);
    if (!debugFoldout.IsFolded()) {
        Editor::BeginIndent(rect, Editor::LargeGap());

        // World Position
        std::ostringstream oss;
        oss.precision(2);
        oss << "World Position: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ", " << GetWorldPosition()
                .z
                << ")" << std::endl;
        rect.y += Editor::TextSize();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();

        // Matrix
        const Matrix transform = GetTransformMatrix();
        oss.str("");
        oss << transform.m0 << ", " << transform.m4 << ", " << transform.m8 << ", " << transform.m12 << std::endl;
        oss << transform.m1 << ", " << transform.m5 << ", " << transform.m9 << ", " << transform.m13 << std::endl;
        oss << transform.m2 << ", " << transform.m6 << ", " << transform.m10 << ", " << transform.m14 << std::endl;
        oss << transform.m3 << ", " << transform.m7 << ", " << transform.m11 << ", " << transform.m15 << std::endl;
        rect.y += Editor::TextSize();
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 4.0f}, oss.str().c_str());
        rect.y += Editor::TextSize() * 4 + Editor::SmallGap();

        Editor::EndIndent(rect, Editor::LargeGap());
    }

    height = rect.y - originalY;
}


float TransformComponent::GetEditorHeight() const {
    return height;
}


Vector3 TransformComponent::GetWorldPosition() const {
    return Vector3Transform(Vector3{0, 0, 0}, GetTransformMatrix());
}

Vector3 TransformComponent::GetWorldRight() const {
    Vector3 rightPos = Vector3Transform(Vector3{1, 0, 0}, GetTransformMatrix());
    return Vector3Normalize(Vector3Subtract(rightPos, GetWorldPosition()));
}

Vector3 TransformComponent::GetWorldUp() const {
    Vector3 upPos = Vector3Transform(Vector3{0, 1, 0}, GetTransformMatrix());
    return Vector3Normalize(Vector3Subtract(upPos, GetWorldPosition()));
}

Vector3 TransformComponent::GetWorldForward() const {
    Vector3 forwardPos = Vector3Transform(Vector3{0, 0, 1}, GetTransformMatrix());
    return Vector3Normalize(Vector3Subtract(forwardPos, GetWorldPosition()));
}

Vector3 TransformComponent::GetEulerAngles() const {
    return eulerAngles;
}

nlohmann::json TransformComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "TransformComponent";
    j["position"] = {position.x, position.y, position.z};
    j["eulerAngles"] = {eulerAngles.x, eulerAngles.y, eulerAngles.z};
    j["scale"] = {scale.x, scale.y, scale.z};
    return j;
}

void TransformComponent::FromJson(const nlohmann::json &json) {
    position = {json["position"][0], json["position"][1], json["position"][2]};
    eulerAngles = {json["eulerAngles"][0], json["eulerAngles"][1], json["eulerAngles"][2]};
    scale = {json["scale"][0], json["scale"][1], json["scale"][2]};
}

void TransformComponent::SetWorldPosition(const Vector3 target) {
    const Vector3 worldPosition = GetWorldPosition();
    const Vector3 toTarget = target - worldPosition;
    position += toTarget;
}

void TransformComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void TransformComponent::OnEditorDraw(Scene *scene) const {
}


void TransformComponent::EditorStart() {
}

void TransformComponent::OnDrawGizmos(Scene *scene) const {
}

void TransformComponent::OnDrawGizmosSelected(Scene *scene) const {
    rlDisableDepthTest();
    const Vector3 worldPosition = GetWorldPosition();
    const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
    const float distToCamera = Vector3Distance(worldPosition, camera->position);

    // Parent
    auto parentPosition = Vector3{0, 0, 0};
    if (const auto *parent = gameObject->GetParent())
        parentPosition = parent->GetTransform()->GetWorldPosition();
    DrawLine3D(worldPosition, parentPosition, YELLOW);

    // Local Axes
    const float length = distToCamera * 0.1f;
    DrawLine3D(worldPosition, Vector3Add(worldPosition, GetWorldRight() * length), RED);
    DrawLine3D(worldPosition, Vector3Add(worldPosition, GetWorldUp() * length), GREEN);
    DrawLine3D(worldPosition, Vector3Add(worldPosition, GetWorldForward() * length), BLUE);
    const float radius = distToCamera * 0.004f;
    DrawSphere(worldPosition, radius, RED);

    // Name Label
    EndMode3D();
    const Vector2 screenPosition = GetWorldToScreenEx(worldPosition, *camera, scene->screenSpaceRect.width,
                                                      scene->screenSpaceRect.height);
    const float width = Editor::TextWidth(gameObject->GetName());
    const float padding = Editor::SmallGap();
    DrawRectangle(screenPosition.x - padding, screenPosition.y - padding,
                  width + padding * 2, 10 + padding * 2, Fade(BLACK, 0.5f));
    DrawText(gameObject->GetName(), screenPosition.x, screenPosition.y, 10, WHITE);
    BeginMode3D(*camera);
    rlEnableDepthTest();
}

void TransformComponent::EditorUpdate() {
}

Matrix TransformComponent::GetTransformMatrix() const {
    Matrix transform = MatrixIdentity();
    const auto *parent = gameObject->GetParent();
    if (parent != nullptr) {
        transform = parent->GetTransform()->GetTransformMatrix();
    }
    transform = MatrixMultiply(MatrixTranslate(position.x, position.y, position.z), transform);
    transform = MatrixMultiply(MatrixRotateXYZ(eulerAngles), transform);
    transform = MatrixMultiply(MatrixScale(scale.x, scale.y, scale.z), transform);
    return transform;
}
