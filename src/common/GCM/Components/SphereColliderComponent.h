#ifndef SPHERECOLLIDERCOMPONENT_H
#define SPHERECOLLIDERCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/VectorProperty.h"

#include "../../editor/SingleValueProperty.h"


class SphereColliderComponent final : public Component {
public:
    explicit SphereColliderComponent();

    void OnEditorGUI(Rectangle &rect) override;

    void OnEditorDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void OnDrawGizmosBottom(Scene *scene) const override;

    float GetEditorHeight() const override;

    void EditorStart() override;

    void EditorUpdate() override;

    void Update() override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

private:
    float radius = 1.0f;

    // ui
    FloatProperty radiusProperty = FloatProperty(&radius, "Radius");
    FoldoutProperty propertiesFoldout = FoldoutProperty("Properties");
    bool statusWarning = false;
    std::string statusText;
    float height;
};


#endif //SCRIPTCOMPONENT_H
