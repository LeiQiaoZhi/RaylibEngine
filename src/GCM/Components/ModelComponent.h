#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include <string>

#include "../../editor/FoldoutProperty.h"

class ModelComponent final : public Component {
public:
    explicit ModelComponent() {
        headerProperty = ComponentHeaderProperty("Model");

        const auto path = std::string(ASSET_DIR) + "/models/" + "duck_floaty.glb";
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
