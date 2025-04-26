#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/VectorProperty.h"

#include "../../editor/SingleValueProperty.h"


class RigidbodyComponent final : public Component {
public:
    explicit RigidbodyComponent();

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
    enum class RigidbodyType {
        Static,
        Kinematic,
        Dynamic,
    };
    RigidbodyType rigidbodyType = RigidbodyType::Static;
    float mass = 1.0f;
    float drag = 0.0f;

    Vector3 velocity = {0, 0, 0};
    Vector3 acceleration = {0, 0, 0};

    // ui
    FoldoutProperty debugFoldout = FoldoutProperty("Debug");
    FloatProperty massProperty = FloatProperty(&mass, "Mass");
    FloatProperty dragProperty = FloatProperty(&drag, "Drag");
    bool statusWarning = false;
    std::string statusText;
    float height;
};


#endif //RIGIDBODYCOMPONENT_H
