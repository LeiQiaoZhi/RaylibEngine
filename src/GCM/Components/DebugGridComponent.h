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

    void OnDraw(::Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

private:
    Vector2 size = {10, 10};
    Vector2 spacing = {1, 1};
    Vector3 normal = {0, 1, 0};
    Color color = GRAY;

    float editorHeight;
    Vector2Property sizeProperty = Vector2Property(&size, "Size");
    Vector2Property spacingProperty = Vector2Property(&spacing, "Spacing");
    Vector3Property normalProperty = Vector3Property(&normal, "Normal");
    ColorProperty colorProperty = ColorProperty(&color, "Color");
};


#endif //DEBUGGRIDCOMPONENT_H
