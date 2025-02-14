#include "GameObject.h"

void GameObject::AddChild(GameObject *child) {
    child->scene = scene;
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
        if (component->enabled)
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

void GameObject::DrawGizmosBottom(Scene *scene) const {
    // own components draw gizmos
    for (const auto *component: components) {
        if (component->enabled)
            component->OnDrawGizmosBottom(scene);
    }

    // children draw gizmos
    for (const auto *child: children) {
        child->DrawGizmosBottom(scene);
    }
}

void GameObject::DrawGizmos(Scene *scene) const {
    // own components draw gizmos
    for (const auto *component: components) {
        if (component->enabled)
            component->OnDrawGizmos(scene);
    }

    // children draw gizmos
    for (const auto *child: children) {
        child->DrawGizmos(scene);
    }
}

void GameObject::DrawGizmosSelected(Scene *scene) const {
    for (const auto *component: components) {
        if (component->enabled)
            component->OnDrawGizmosSelected(scene);
    }
}

void GameObject::Draw(Scene *scene) const {
    // own components draw
    for (const auto *component: components) {
        if (component->enabled)
            component->OnDraw(scene);
    }

    // children draw
    for (const auto *child: children) {
        child->Draw(scene);
    }
}

void GameObject::StartComponents() const {
    // start own components
    for (auto *component: components) {
        if (component->enabled)
            component->Start();
    }

    // start children
    for (const auto *child: children) {
        child->StartComponents();
    }
}
