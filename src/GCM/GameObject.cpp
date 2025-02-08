//
// Created by Qiaozhi Lei on 2/6/25.
//

#include "GameObject.h"

void GameObject::AddChild(GameObject *child) {
    child->parent = this;
    child->sceneDepth = sceneDepth + 1;
    children.push_back(child);
}

int GameObject::RecursiveChildCount() const {
    int count = 0;
    for (const auto *child: children) {
        count += child->RecursiveChildCount() + 1;
    }
    return count;
}

void GameObject::AddComponent(Component *component) {
    component->gameObject = this;
    components.push_back(component);
}

void GameObject::UpdateComponents() const {
    // update own components
    for (auto *component: components) {
        component->Update();
    }

    // update children
    for (const auto *child: children) {
        child->UpdateComponents();
    }
}

GameObject *GameObject::GetGameObjectByUID(int uid) {
    if (this->uid == uid) {
        return this;
    }

    for (auto *child: children) {
        auto *result = child->GetGameObjectByUID(uid);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

void GameObject::DrawGizmos(Scene *scene) const {
    // own components draw gizmos
    for (const auto *component: components) {
        component->OnDrawGizmos(scene);
    }

    // children draw gizmos
    for (const auto *child: children) {
        child->DrawGizmos(scene);
    }
}

void GameObject::DrawGizmosSelected(Scene *scene) const {
    for (const auto *component: components) {
        component->OnDrawGizmosSelected(scene);
    }
}
