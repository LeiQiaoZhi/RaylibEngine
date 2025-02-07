#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"


class CameraComponent final : public Component {
public:
    explicit CameraComponent(Camera *camera, const int mode) : camera(camera), cameraMode(mode) {
    }

    void EditorUpdate(Rectangle &rect) override;

    void Update() override;

    Camera *GetRaylibCamera() const {
        return camera;
    }

private:
    Camera *camera = nullptr;
    int cameraMode;
};


#endif //CAMERACOMPONENT_H
