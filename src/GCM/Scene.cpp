#include "Scene.h"
#include "GameObject.h"

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

void Scene::DrawGizmos(Scene *scene) const {
    root->DrawGizmos(scene);

    // draw gizmos for selected game object
    if (selectedGameObjectUID != -1) {
        const auto *selectedGameObject = GetGameObjectByUID(selectedGameObjectUID);
        if (selectedGameObject) {
            selectedGameObject->DrawGizmosSelected(scene);
        }
    }
}

void Scene::Draw(Scene *scene) {
    root->Draw(scene);
}

void Scene::DrawGizmosBottom(Scene *scene) {
    root->DrawGizmosBottom(scene);
}

void Scene::StartComponents() const {
    root->StartComponents();
}

void Scene::SetRoot(GameObject *gameObject) {
    root = gameObject;
    root->scene = this;
}
