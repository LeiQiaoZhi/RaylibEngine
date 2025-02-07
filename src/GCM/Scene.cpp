//
// Created by Qiaozhi Lei on 2/6/25.
//

#include "Scene.h"

int Scene::GameObjectCount() const {
    return root->RecursiveChildCount() + 1;
}

CameraComponent *Scene::GetMainCamera() const {
    return root->GetComponentInChildren<CameraComponent>();
}

void Scene::UpdateComponents() const {
    root->UpdateComponents();
}
