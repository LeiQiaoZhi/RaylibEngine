#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"
#include "raylib.h"
#include "../../editor/ColorProperty.h"
#include "../../editor/MaterialProperty.h"


class CameraComponent final : public Component {
public:
    explicit CameraComponent() {
        headerProperty.label = "Camera";
        camera = new Camera();
        camera->position = (Vector3){10.0f, 100.0f, 10.0f};
        camera->target = (Vector3){0.0f, 0.0f, 0.0f};
        camera->up = (Vector3){0.0f, 1.0f, 0.0f};
        camera->fovy = 45.0f;

        const Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(cube);
    }

    ~CameraComponent() override {
        delete camera;
    }

    void OnEditorGUI(Rectangle &rect) override;

    float GetEditorHeight() const override;

    void OnEditorDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void EditorStart() override;

    void EditorUpdate() override;

    Camera *GetRaylibCamera() const {
        return camera;
    }

    void SetCameraMode(const int mode) {
        cameraMode = mode;
    }

    void OnDrawGizmosBottom(Scene *scene) const override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    void DrawBackground();

private:
    enum class BackgroundMode {
        Color,
        Skybox
    };

    Camera *camera = nullptr;
    int cameraMode;

    // Background
    BackgroundMode backgroundMode = BackgroundMode::Color;
    Color backgroundColor = RAYWHITE;
    Model skybox;
    bool skyboxFlipY = false;

    // UI
    ColorProperty backgroundColorProperty = ColorProperty(&backgroundColor, "Background Color", false);
    MaterialProperty skyboxMaterialProperty = MaterialProperty(&skybox, 0);
    float height;
    bool updating = false;
};


#endif //CAMERACOMPONENT_H
