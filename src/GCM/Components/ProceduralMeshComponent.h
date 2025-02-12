#ifndef PROCEUDALMESHCOMPONENT_H
#define PROCEUDALMESHCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "ModelComponent.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/MaterialProperty.h"
#include "../../editor/VectorProperty.h"

class ProceduralMeshComponent final : public Component {
public:
    explicit ProceduralMeshComponent() {
        headerProperty.label = "Procedural Mesh";
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

    void OnDrawGizmosBottom(Scene *scene) const override;

private:
    enum class MeshType : unsigned int {
        Cube,
        Plane,
    };

    ModelComponent *modelComponent = nullptr;
    MeshType meshType = MeshType::Cube;
    Vector3 cubeSize = {10, 10, 10};
    Vector3 cubeWorldSize = {10, 10, 10};
    Vector2 planeSize = {10, 10};
    Vector2 planeWorldSize = {10, 10};

    Vector3Property sizeProperty = Vector3Property(&cubeSize, "Size");
    Vector3Property worldSizeProperty = Vector3Property(&cubeWorldSize, "World Size");
    Vector2Property planeSizeProperty = Vector2Property(&planeSize, "Size");
    Vector2Property planeWorldSizeProperty = Vector2Property(&planeWorldSize, "World Size");

    std::string warningText;
    float height;
};


#endif //PROCEUDALMESHCOMPONENT_H
