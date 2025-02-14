#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "../../editor/FoldoutProperty.h"
#include "../../editor/MaterialProperty.h"

class ModelComponent final : public Component {
public:
    explicit ModelComponent() {
        headerProperty.label = "Model";

        // const auto path = std::string(ASSET_DIR) + "/models/" + "duck_floaty.glb";
        const auto path = std::string(SHADER_EXAMPLES_ASSET_DIR) + "/models/old_car_new.glb";
        LoadModelFromFile(path); // TODO: remove this
    }

    ~ModelComponent() override {
        if (model) {
            UnloadModel(*model);
            delete model;
        }
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

    void LoadModelFromFile(const std::string &filename);

    void SetModelFromMesh(const Mesh &mesh);

    void OnDrawGizmosBottom(Scene *scene) const override;

public:
    Model *model = nullptr;

private:
    std::string warningText;
    char filename[32];
    bool editMode;
    float height;

    MaterialProperty materialProp = MaterialProperty(model, 0);
    FoldoutProperty debugFoldout = FoldoutProperty("Debug");
    bool drawSurface = true;
    bool drawBounds = true;
    bool drawWireframe = false;

    enum class RenderType: unsigned int {
        Default,
        DebugUV,
        DebugNormal
    };

    RenderType renderType = RenderType::Default;
    // TODO: more materials
};


#endif //MODELCOMPONENT_H
