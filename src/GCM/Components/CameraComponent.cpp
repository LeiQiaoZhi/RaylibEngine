//
// Created by Qiaozhi Lei on 2/7/25.
//

#include "CameraComponent.h"
#include "raylib.h"

void CameraComponent::EditorUpdate() {
}

void CameraComponent::Update() {
    UpdateCamera(camera, cameraMode);
}
