#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "raylib.h"


class CameraComponent final : public Component {
public:
    explicit CameraComponent(Camera *camera, const int mode) : camera(camera), cameraMode(mode) {
        headerProperty.label = "Camera";
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void Start() override;

    void Update() override;

    Camera *GetRaylibCamera() const {
        return camera;
    }

    void OnDrawGizmosBottom(Scene *scene) const override;

private:
    Camera *camera = nullptr;
    int cameraMode;
};


#endif //CAMERACOMPONENT_H
