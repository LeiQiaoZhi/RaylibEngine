#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/VectorProperty.h"


class TransformComponent final : public Component {
public:
    explicit TransformComponent() {
        headerProperty.label = "Transform";
    }

    void OnEditorGUI(Rectangle &rect) override;

    void OnEditorDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void OnDrawGizmosBottom(Scene *scene) const override;

    float GetEditorHeight() const override;

    void EditorStart() override;

    void EditorUpdate() override;

    Matrix GetTransformMatrix() const;

    Vector3 GetWorldPosition() const;

    Vector3 GetWorldRight() const;

    Vector3 GetWorldUp() const;

    Vector3 GetWorldForward() const;

    Vector3 GetEulerAngles() const;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    Vector3 GetLocalPosition() const {
        return position;
    }

    void SetLocalPosition(const Vector3 &pos) {
        position = pos;
    }

    void SetWorldPosition(Vector3 target);

private:
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 eulerAngles = {0.0f, 0.0f, 0.0f};
    Vector3 scale = {1.0f, 1.0f, 1.0f};
    Vector3Property positionProperty = Vector3Property(&position, "position");
    Vector3Property eulerAnglesProperty = Vector3Property(&eulerAngles, "rotation");
    Vector3Property scaleProperty = Vector3Property(&scale, "scale");
    FoldoutProperty debugFoldout = FoldoutProperty("Debug", true);
    float height;
};


#endif //TRANSFORMCOMPONENT_H
