#ifndef COMPONENT_H
#define COMPONENT_H
#include <typeindex>

#include "raylib.h"
#include "../../editor/ComponentHeaderProperty.h"

class Scene;
class GameObject;

class Component {
public:
    // every frame to render in inspector view
    virtual void OnEditorGUI(Rectangle &rect) = 0;

    // height of the component in inspector view
    virtual float GetEditorHeight() const = 0;

    // render in scene view every frame
    virtual void OnDraw(Scene *scene) const = 0;

    // when gizmos are enabled, render gizmos every frame
    virtual void OnDrawGizmos(Scene *scene) const = 0;

    // when gizmos are enabled, render gizmos every frame when selected
    virtual void OnDrawGizmosSelected(Scene *scene) const = 0;

    virtual void Start() = 0;

    // TODO: differentiate between Editor and Game mode
    virtual void Update() = 0;

    //    virtual void OnEnable() = 0;
    //    virtual void OnDisable() = 0;
    //    virtual void OnDestroy() = 0;
    virtual ~Component() = default;

    virtual void OnDrawGizmosBottom(Scene *scene) const = 0;

    virtual nlohmann::json ToJson() const = 0;

    virtual void FromJson(const nlohmann::json &json) = 0;

    virtual std::vector<std::type_index> Dependencies() { return {}; }

    static std::map<std::string, std::function<Component *()> > &ComponentTypeMap();

    static std::vector<std::string> &GetAvailableComponentTypes();

public:
    GameObject *gameObject = nullptr;
    bool enabled = true;
    // need this to prevent error since we are removing components in the middle of GUI iteration
    bool removed = false;

protected:
    friend class GameObject;
    ComponentHeaderProperty headerProperty = ComponentHeaderProperty("Component", &enabled, this);
};


#endif //COMPONENT_H
