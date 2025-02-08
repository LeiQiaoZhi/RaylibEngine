#ifndef COMPONENT_H
#define COMPONENT_H
#include "raylib.h"

class Scene;
class GameObject;

class Component {
public:
    // every frame to render in inspector view
    virtual void OnEditorGUI(Rectangle &rect) = 0;

    // height of the component in inspector view
    virtual float GetEditorHeight() const = 0;

    // when gizmos are enabled, render gizmos every frame
    virtual void OnDrawGizmos(Scene* scene) const = 0;

    // when gizmos are enabled, render gizmos every frame when selected
    virtual void OnDrawGizmosSelected(Scene* scene) const = 0;

    //    virtual void Start() = 0;

    // TODO: differentiate between Editor and Game mode
    virtual void Update() = 0;

    //    virtual void OnEnable() = 0;
    //    virtual void OnDisable() = 0;
    //    virtual void OnDestroy() = 0;
    virtual ~Component() = default;

protected:
    friend class GameObject;
    GameObject *gameObject = nullptr;
};


#endif //COMPONENT_H
