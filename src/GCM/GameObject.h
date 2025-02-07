#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <utility>
#include <vector>

#include "Components/Component.h"


class GameObject {
public:
    explicit GameObject(std::string name, const int uid) : name(std::move(name)), uid(uid) {
    }

    void Destroy();


    const char *GetName() const {
        return name.c_str();
    }

    int GetSceneDepth() const {
        return sceneDepth;
    }

    // Children
    void AddChild(GameObject *child);

    int RecursiveChildCount() const;

    const std::vector<GameObject *> &GetChildren() const {
        return children;
    }

    // Component
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

    void UpdateComponents() const;

private:
    std::string name;
    int uid;
    int sceneDepth = 0;

    GameObject *parent = nullptr;
    std::vector<GameObject *> children = {};
    std::vector<Component *> components = {};
};


#endif //GAMEOBJECT_H
