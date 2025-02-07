#ifndef COMPONENT_H
#define COMPONENT_H
#include "raylib.h"

class GameObject;

class Component {
public:
    virtual void EditorUpdate(Rectangle &rect) = 0;

    //    virtual void Start() = 0;
    virtual void Update() = 0;

    //    virtual void OnEnable() = 0;
    //    virtual void OnDisable() = 0;
    //    virtual void OnDestroy() = 0;
    virtual ~Component() = default;

private:
    friend class GameObject;
    GameObject *gameObject = nullptr;
};


#endif //COMPONENT_H
