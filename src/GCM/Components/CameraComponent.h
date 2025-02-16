#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "raylib.h"


class CameraComponent final : public Component {
public:
    explicit CameraComponent() {
        headerProperty.label = "Camera";
        camera = new Camera();
        camera->position = (Vector3){10.0f, 10.0f, 10.0f};
        camera->target = (Vector3){0.0f, 0.0f, 0.0f};
        camera->up = (Vector3){0.0f, 1.0f, 0.0f};
        camera->fovy = 45.0f;
    }

    ~CameraComponent() override {
        delete camera;
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

    void SetCameraMode(const int mode) {
        cameraMode = mode;
    }

    void OnDrawGizmosBottom(Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

private:
    Camera *camera = nullptr;
    int cameraMode;
};


#endif //CAMERACOMPONENT_H
