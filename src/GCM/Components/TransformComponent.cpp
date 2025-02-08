#include "TransformComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"

void TransformComponent::OnEditorGUI(Rectangle &rect) {
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    positionProperty.OnEditorGUI(rect);
    eulerAnglesProperty.OnEditorGUI(rect);
    scaleProperty.OnEditorGUI(rect);

    std::ostringstream oss;
    oss.precision(2);
    oss << "World Position: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ", " << GetWorldPosition().z
            << ")" << std::endl;
    rect.y += Editor::TextSize();
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, oss.str().c_str());
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // display matrix
    Matrix transform = GetTransformMatrix();
    oss.str("");
    oss << transform.m0 << ", " << transform.m4 << ", " << transform.m8 << ", " << transform.m12 << std::endl;
    oss << transform.m1 << ", " << transform.m5 << ", " << transform.m9 << ", " << transform.m13 << std::endl;
    oss << transform.m2 << ", " << transform.m6 << ", " << transform.m10 << ", " << transform.m14 << std::endl;
    oss << transform.m3 << ", " << transform.m7 << ", " << transform.m11 << ", " << transform.m15 << std::endl;
    rect.y += Editor::TextSize();
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 4.0f}, oss.str().c_str());
    rect.y += Editor::TextSize() * 4 + Editor::SmallGap();
}

void TransformComponent::OnDraw(Scene *scene) const {
}


float TransformComponent::GetEditorHeight() const {
    return Editor::TextSize() * 7 + Editor::SmallGap() * 3
           + headerProperty.GetEditorHeight()
           + positionProperty.GetEditorHeight()
           + eulerAnglesProperty.GetEditorHeight()
           + scaleProperty.GetEditorHeight();
}

void TransformComponent::Start() {
}

void TransformComponent::OnDrawGizmos(Scene *scene) const {
    rlDisableDepthTest();
    const Vector3 worldPosition = GetWorldPosition();
    DrawSphere(worldPosition, 1.0f, RED);
    rlEnableDepthTest();
}

void TransformComponent::OnDrawGizmosSelected(Scene *scene) const {
    rlDisableDepthTest();
    const Vector3 worldPosition = GetWorldPosition();
    auto parentPosition = Vector3{0, 0, 0};
    if (const auto *parent = gameObject->GetParent()) {
        parentPosition = parent->GetTransform()->GetWorldPosition();
    }
    DrawLine3D(worldPosition, parentPosition, YELLOW);

    constexpr float length = 10.0f;
    Vector3 right = Vector3Subtract(Vector3Transform(Vector3{1, 0, 0}, GetTransformMatrix()), worldPosition);
    Vector3 up = Vector3Subtract(Vector3Transform(Vector3{0, 1, 0}, GetTransformMatrix()), worldPosition);
    Vector3 forward = Vector3Subtract(Vector3Transform(Vector3{0, 0, 1}, GetTransformMatrix()), worldPosition);
    DrawLine3D(worldPosition, Vector3Add(worldPosition, Vector3Scale(right, length)), RED);
    DrawLine3D(worldPosition, Vector3Add(worldPosition, Vector3Scale(up, length)), GREEN);
    DrawLine3D(worldPosition, Vector3Add(worldPosition, Vector3Scale(forward, length)), BLUE);

    EndMode3D();
    const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
    const Vector2 screenPosition = GetWorldToScreenEx(worldPosition, *camera, 800, 900);
    DrawText(gameObject->GetName(), screenPosition.x, screenPosition.y, 10, WHITE);
    rlEnableDepthTest();
    BeginMode3D(*camera);
}

void TransformComponent::Update() {
}

Matrix TransformComponent::GetTransformMatrix() const {
    Matrix transform = MatrixIdentity();
    const auto *parent = gameObject->GetParent();
    if (parent != nullptr) {
        transform = parent->GetTransform()->GetTransformMatrix();
    }
    transform = MatrixMultiply(MatrixScale(scale.x, scale.y, scale.z), transform);
    transform = MatrixMultiply(MatrixRotateXYZ(eulerAngles), transform);
    transform = MatrixMultiply(MatrixTranslate(position.x, position.y, position.z), transform);
    return transform;
}

Vector3 TransformComponent::GetWorldPosition() const {
    return Vector3Transform(Vector3{0, 0, 0}, GetTransformMatrix());
}
