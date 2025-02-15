#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/ColorProperty.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/SingleValueProperty.h"
#include "../../editor/VectorProperty.h"


class LightComponent final : public Component {
public:
    explicit LightComponent() {
        headerProperty.label = "Light";
    }

    void OnEditorGUI(Rectangle &rect) override;

    void OnDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    float GetEditorHeight() const override;

    void Start() override;

    void Update() override;

    void OnDrawGizmosBottom(Scene *scene) const override;

public:
    enum class LightType {
        Ambient = 0,
        Directional,
        Point,
    };

private:
    friend class Scene;
    LightType lightType = LightType::Ambient;
    float intensity = 1.0f;
    float range = 10.0f;
    Color color;

    ColorProperty colorProperty = ColorProperty(&color, "Color", false);
    FloatSlider intensityProperty = FloatSlider(&intensity, "Intensity", 0.0f, 10.0f);
    FloatSlider rangeProperty = FloatSlider(&range, "Range", 0.0f, 100.0f);
    float height;
};


#endif //LIGHTCOMPONENT_H
