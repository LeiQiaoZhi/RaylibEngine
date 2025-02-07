#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <utility>
#include <vector>


class GameObject {
public:
    explicit GameObject(std::string name, int uid) : name(std::move(name)), uid(uid) {
    }

    void Destroy();

    void AddChild(GameObject *child) {
        child->parent = this;
        child->sceneDepth = sceneDepth + 1;
        children.push_back(child);
    }

    int RecursiveChildCount() const;

    const char *GetName() const {
        return name.c_str();
    }

    int GetSceneDepth() const {
        return sceneDepth;
    }

    const std::vector<GameObject *> &GetChildren() const {
        return children;
    }

private:
    std::string name;
    int uid;
    int sceneDepth = 0;
    GameObject *parent = nullptr;
    std::vector<GameObject *> children = {};
};


#endif //GAMEOBJECT_H
