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

    void LoadModelFromFile(const std::string &path);

    void SetModelFromMesh(const Mesh &mesh);

    void OnDrawGizmosBottom(Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    Model *model = nullptr;

private:
    enum class RenderType: unsigned int {
        Default,
        DebugUV,
        DebugNormal
    };

    // States
    char filename[32];
    bool drawSurface = true;
    bool drawBounds = true;
    bool drawWireframe = false;
    RenderType renderType = RenderType::Default;

    // UI
    FoldoutProperty materialsFoldout = FoldoutProperty("Materials");
    std::vector<MaterialProperty> materialProps = {};
    FoldoutProperty debugFoldout = FoldoutProperty("Debug");
    bool editMode;
    std::string statusText;
    bool statusWarning = false;
    float height;
};


#endif //MODELCOMPONENT_H
