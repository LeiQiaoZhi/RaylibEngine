#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "../../editor/FoldoutProperty.h"

class ModelComponent final : public Component {
public:
    explicit ModelComponent() {
        headerProperty.label = "Model";

        // const auto path = std::string(ASSET_DIR) + "/models/" + "duck_floaty.glb";
        const auto path = std::string(SHADER_EXAMPLES_ASSET_DIR) + "/models/old_car_new.glb";
        LoadModelFromFile(path); // TODO: remove this
        model->meshMaterial[0] = 0;
        model->materials[0].maps[MATERIAL_MAP_ALBEDO].color = RED;
        const auto texPath = std::string(SHADER_EXAMPLES_ASSET_DIR) + "/old_car_d.png";
        model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = LoadTexture(texPath.c_str());
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

    void OnDrawGizmosBottom(Scene *scene) const override;

private:
    Model *model = nullptr;
    std::string warningText;
    BoundingBox bounds;
    char filename[32];
    bool editMode;
    float height;

    FoldoutProperty debugFoldout = FoldoutProperty("Debug");
    bool drawSurface = true;
    bool drawBounds = true;
    bool drawWireframe = false;
};


#endif //MODELCOMPONENT_H
