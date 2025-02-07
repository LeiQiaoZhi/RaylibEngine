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
    for (auto *child: children) {
        child->UpdateComponents();
    }
}



