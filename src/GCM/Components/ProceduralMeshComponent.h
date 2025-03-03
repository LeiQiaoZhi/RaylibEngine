#ifndef PROCEUDALMESHCOMPONENT_H
#define PROCEUDALMESHCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "ModelComponent.h"
#include "../../editor/SingleValueProperty.h"
#include "../../editor/VectorProperty.h"

class ProceduralMeshComponent final : public Component {
public:
    explicit ProceduralMeshComponent() {
        headerProperty.label = "Procedural Mesh";
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnEditorDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void EditorStart() override;

    void EditorUpdate() override;

    void OnDrawGizmosBottom(Scene *scene) const override;

    void GenerateMesh();

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    std::vector<std::type_index> Dependencies() override {
        return {std::type_index(typeid(ModelComponent))};
    }

public:
    enum class MeshType : unsigned int {
        Cube,
        Plane,
        Sphere,
    };

    MeshType meshType = MeshType::Cube;
    Vector3 cubeSize = {10, 10, 10};
    Vector3 cubeWorldSize = {10, 10, 10};
    Vector3Property sizeProperty = Vector3Property(&cubeSize, "Size");
    Vector3Property worldSizeProperty = Vector3Property(&cubeWorldSize, "World Size");

private:
    ModelComponent *modelComponent = nullptr;
    bool createOnStart = false;

    // plane
    Vector2 planeSize = {10, 10};
    Vector2 planeWorldSize = {10, 10};
    Vector2Property planeSizeProperty = Vector2Property(&planeSize, "Size");
    Vector2Property planeWorldSizeProperty = Vector2Property(&planeWorldSize, "World Size");

    // sphere
    float sphereRadius = 1;
    int sphereRings = 16;
    int sphereSlices = 16;
    FloatSlider sphereRadiusProperty = FloatSlider(&sphereRadius, "Radius", 0, 100);
    IntSlider sphereRingsProperty = IntSlider(&sphereRings, "Rings", 0, 100);
    IntSlider sphereSlicesProperty = IntSlider(&sphereSlices, "Slices", 0, 100);

    std::string statusText;
    bool statusWarning = false;
    float height;
};


#endif //PROCEUDALMESHCOMPONENT_H
