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

        highlightedShader = LoadShader(
            (std::string(INTERNAL_ASSET_DIR) + "/shaders/default.vert").c_str(),
            (std::string(INTERNAL_ASSET_DIR) + "/shaders/highlight.frag").c_str());
        if (highlightedShader.id == 0) {
            statusText = "Shader error, not loaded";
            statusWarning = true;
        }
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

    void SetHighlightedMesh(int index);

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
    int highlightedMeshIndex = -1;
    bool isPicking = false;
    Material highlightedMaterial;
    Shader highlightedShader;
    Shader originalShader;
};



#endif //MODELCOMPONENT_H
