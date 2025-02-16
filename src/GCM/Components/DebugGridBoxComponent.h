#ifndef DEBUGGRIDBOXCOMPONENT_H
#define DEBUGGRIDBOXCOMPONENT_H

#include "Component.h"
#include "../../editor/ColorProperty.h"
#include "../../editor/VectorProperty.h"

class DebugGridBoxComponent final : public Component {
public:
    DebugGridBoxComponent() {
        headerProperty.label = "Debug Grid Box";
    }

    ~DebugGridBoxComponent() override = default;

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

    void OnDrawGizmosBottom(::Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

private:
    Vector3 size = {40, 40, 40};
    Vector3 spacing = {10, 10, 10};
    Color color = {255, 255, 255, 50};

    float editorHeight;
    Vector3Property sizeProperty = Vector3Property(&size, "Size");
    Vector3Property spacingProperty = Vector3Property(&spacing, "Spacing");
    ColorProperty colorProperty = ColorProperty(&color, "Color");
};


#endif //DEBUGGRIDBOXCOMPONENT_H
