#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/Vector3Property.h"


class TransformComponent final : public Component {
public:
    explicit TransformComponent() {
        headerProperty = ComponentHeaderProperty("Transform");
    }

    void OnEditorGUI(Rectangle &rect) override;

    void OnDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    float GetEditorHeight() const override;

    void Start() override;

    void Update() override;

    Matrix GetTransformMatrix() const;

    Vector3 GetWorldPosition() const;

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
