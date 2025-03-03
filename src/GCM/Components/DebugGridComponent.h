#ifndef DEBUGGRIDCOMPONENT_H
#define DEBUGGRIDCOMPONENT_H

#include "Component.h"
#include "../../editor/ColorProperty.h"
#include "../../editor/VectorProperty.h"

class DebugGridComponent final : public Component {
public:
    DebugGridComponent() {
        headerProperty.label = "Debug Grid";
    }

    ~DebugGridComponent() override = default;

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnEditorDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void EditorStart() override;

    void EditorUpdate() override;

    void OnDrawGizmosBottom(::Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

private:
    // states
    Vector2 size = {10, 10};
    Vector2 spacing = {1, 1};
    Color color = {255, 255, 255, 50};

    // ui
    float editorHeight;
    Vector2Property sizeProperty = Vector2Property(&size, "Size");
    Vector2Property spacingProperty = Vector2Property(&spacing, "Spacing");
    ColorProperty colorProperty = ColorProperty(&color, "Color");
};


#endif //DEBUGGRIDCOMPONENT_H
