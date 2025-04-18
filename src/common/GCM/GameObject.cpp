#include "GameObject.h"

#include <sstream>

#include "Components/DebugGridBoxComponent.h"
#include "Components/DebugGridComponent.h"
#include "Components/JelloComponent.h"
#include "Components/ModelComponent.h"
#include "Components/ProceduralMeshComponent.h"

GameObject::GameObject(nlohmann::json j, Scene *scene) {
    this->scene = scene;
    name = j["name"];
    uid = j["uid"];
    sceneDepth = j["sceneDepth"];

    // children
    for (const auto &childJson: j["children"]) {
        AddChild(new GameObject(childJson, scene));
    }

    // components
    for (const auto &componentJson: j["components"]) {
        if (!componentJson.contains("type"))
            continue;
        const std::string type = componentJson["type"];
        const std::function<Component *()> componentFactory = Component::ComponentTypeMap()[type];
        Component *component = componentFactory();
        if (type == "TransformComponent") {
            transform = dynamic_cast<TransformComponent *>(component);
        }
        if (component) {
            component->enabled = componentJson["enabled"];
            if (componentJson.contains("header"))
                component->headerProperty.FromJson(componentJson["header"]);
            component->FromJson(componentJson);
            AddComponent(component);
        }
    }
}

GameObject::~GameObject() {
    parent = nullptr;
    scene = nullptr;
    transform = nullptr;

    for (const auto *child: children) {
        delete child;
    }
    children.clear();
    for (const auto *component: components) {
        delete component;
    }
    components.clear();
}

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

    // start is called when all objs are loaded in scene load
    // component->EditorStart();
}

void GameObject::EditorUpdate() const {
    // update own components
    for (auto *component: components) {
        if (component->enabled)
            component->EditorUpdate();
    }

    // update children
    for (const auto *child: children) {
        child->EditorUpdate();
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

void GameObject::Start() const {
    // start own components
    for (auto *component: components) {
        if (component->enabled)
            component->Start();
    }

    // start children
    for (const auto *child: children) {
        child->Start();
    }
}

void GameObject::Update() const {
    // update own components
    for (auto *component: components) {
        if (component->enabled)
            component->Update();
    }

    // update children
    for (const auto *child: children) {
        child->Update();
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

void GameObject::EditorDraw(Scene *scene) const {
    // own components draw
    for (const auto *component: components) {
        if (component->enabled)
            component->OnEditorDraw(scene);
    }

    // children draw
    for (const auto *child: children) {
        child->EditorDraw(scene);
    }
}

void GameObject::EditorStart() const {
    // start own components
    for (auto *component: components) {
        if (component->enabled)
            component->EditorStart();
    }

    // start children
    for (const auto *child: children) {
        child->EditorStart();
    }
}

nlohmann::json GameObject::ToJson() const {
    nlohmann::json j;
    j["uid"] = uid;
    j["name"] = name;
    j["sceneDepth"] = sceneDepth;

    std::vector<nlohmann::json> childrenJson;
    for (const auto *child: children) {
        childrenJson.emplace_back(child->ToJson());
    }

    std::vector<nlohmann::json> componentsJson;
    for (const auto *component: components) {
        nlohmann::json componentJson = component->ToJson();
        componentJson["enabled"] = component->enabled;
        componentJson["header"] = component->headerProperty.ToJson();
        componentsJson.emplace_back(componentJson);
    }

    j["components"] = componentsJson;
    j["children"] = childrenJson;

    return j;
}

void GameObject::TryAddComponent(Component *component, std::string *status_text, bool *status_warning) {
    if (typeid(*component) == typeid(TransformComponent)) {
        if (transform) {
            if (status_text != nullptr)
                *status_text = "Transform component already exists";
            if (status_warning != nullptr)
                *status_warning = true;
            return;
        }
        transform = dynamic_cast<TransformComponent *>(component);
    } else {
        AddComponent(component);
        if (status_text != nullptr)
            *status_text = "Component added";
        if (status_warning != nullptr)
            *status_warning = false;
    }
}

void GameObject::TryRemoveComponent(Component *component, std::string *status_text, bool *status_warning) {
    if (typeid(*component) == typeid(TransformComponent)) {
        if (transform) {
            *status_text = "Transform component cannot be removed";
            *status_warning = true;
            return;
        }
        transform = dynamic_cast<TransformComponent *>(component);
    } else {
        const auto toRemoveIterator = std::find(components.begin(), components.end(), component);
        if (toRemoveIterator != components.end()) {
            // Check if other components depend on this component
            std::vector<Component *> dependentComponents;
            for (auto *c: components) {
                for (const auto &dependency: c->Dependencies()) {
                    if (std::type_index(typeid(*component)) == dependency) {
                        dependentComponents.push_back(c);
                    }
                }
            }
            if (!dependentComponents.empty()) {
                std::ostringstream oss;
                oss << "[";
                for (int i = 0; i < dependentComponents.size(); ++i) {
                    oss << dependentComponents[i]->headerProperty.label;
                    if (i < dependentComponents.size() - 1)
                        oss << ", ";
                }
                oss << "] depend on this component";
                *status_text = oss.str();
                *status_warning = true;
                return;
            }

            component->removed = true;
            // still need to delete the component at a safe time
            *status_text = "Component removed";
            *status_warning = false;
        } else {
            *status_text = "Component not found";
            *status_warning = true;
        }
    }
}

void GameObject::RemoveComponent(Component *component) {
    const auto toRemoveIterator = std::find(components.begin(), components.end(), component);
    if (toRemoveIterator != components.end()) {
        components.erase(toRemoveIterator);
        delete component;
    }
}

void GameObject::RemoveChild(GameObject *child) {
    // remove child from children
    const auto toRemoveIterator = std::find(children.begin(), children.end(), child);
    if (toRemoveIterator != children.end()) {
        children.erase(toRemoveIterator);
        delete child;
    }
}
