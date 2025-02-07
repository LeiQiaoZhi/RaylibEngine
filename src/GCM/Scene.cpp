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

GameObject *Scene::GetGameObjectByUID(const int uid) const {
    if (uid < 0)
        return nullptr;

    return root->GetGameObjectByUID(uid);
}
