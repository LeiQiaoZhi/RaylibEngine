#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <utility>
#include <vector>

#include "Scene.h"
#include "Components/Component.h"
#include "Components/TransformComponent.h"


class GameObject {
public:
    explicit GameObject(std::string name, const int uid) : name(std::move(name)), uid(uid) {
        AddComponent(transform);
    }

    void Destroy();


    const char *GetName() const {
        return name.c_str();
    }

    int GetSceneDepth() const {
        return sceneDepth;
    }

    int GetUID() const {
        return uid;
    }

    // Children and Parent
    void AddChild(GameObject *child);

    int RecursiveChildCount() const;

    const std::vector<GameObject *> &GetChildren() const {
        return children;
    }

    GameObject *GetParent() const {
        return parent;
    }

    // Component
    TransformComponent *GetTransform() const {
        return transform;
    }

    const std::vector<Component *> &GetComponents() const {
        return components;
    }

    void AddComponent(Component *component);

    template<typename T>
    T *GetComponent() {
        for (auto *component: components) {
            if (T *casted = dynamic_cast<T *>(component)) {
                return casted;
            }
        }
        return nullptr;
    }

    template<typename T>
    T *GetComponentInChildren() {
        if (auto *component = GetComponent<T>()) {
            return component;
        }

        for (auto *child: children) {
            auto *component = child->GetComponentInChildren<T>();
            if (component) {
                return component;
            }
        }
        return nullptr;
    }

    template<typename T>
    std::vector<T *> GetComponentsInChildren() {
        std::vector<T *> result = {};
        if (auto *component = GetComponent<T>()) {
            result.push_back(component);
        }

        for (auto *child: children) {
            std::vector<T *> components = child->GetComponentsInChildren<T>();
            if (!components.empty()) {
                result.insert(result.end(), components.begin(), components.end());
            }
        }
        return result;
    }


    void UpdateComponents() const;

    GameObject *GetGameObjectByUID(int uid);

    void DrawGizmosBottom(Scene *scene) const;

    void DrawGizmos(Scene *scene) const;

    void DrawGizmosSelected(Scene *scene) const;

    void Draw(Scene *scene) const;

    void StartComponents() const;

protected:
    friend class Scene;
    std::string name;
    int uid;
    int sceneDepth = 0;

    Scene *scene = nullptr;
    GameObject *parent = nullptr;
    TransformComponent *transform = new TransformComponent();
    std::vector<GameObject *> children = {};
    std::vector<Component *> components = {};
};


#endif //GAMEOBJECT_H
