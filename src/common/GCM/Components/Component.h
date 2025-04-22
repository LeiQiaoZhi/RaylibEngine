#ifndef COMPONENT_H
#define COMPONENT_H
#include <typeindex>

#include "raylib.h"
#include "../../editor/ComponentHeaderProperty.h"

class Scene;
class GameObject;
class Logger;

class Component {
public:
    // every frame to render in inspector view
    virtual void OnEditorGUI(Rectangle &rect) = 0;

    // height of the component in inspector view
    virtual float GetEditorHeight() const = 0;

    // render in scene view every frame
    virtual void OnEditorDraw(Scene *scene) const = 0;

    // when gizmos are enabled, render gizmos every frame
    virtual void OnDrawGizmos(Scene *scene) const = 0;

    // when gizmos are enabled, render gizmos every frame when selected
    virtual void OnDrawGizmosSelected(Scene *scene) const = 0;

    virtual void EditorStart() = 0;

    virtual void EditorUpdate() = 0;

    // runtime events

    virtual void OnDraw(Scene *scene) const {
        OnEditorDraw(scene);
    }

virtual void Start() {
        EditorStart();
    }

    virtual void Update() {
        EditorUpdate();
    }

    //    virtual void OnEnable() = 0;
    //    virtual void OnDisable() = 0;
    //    virtual void OnDestroy() = 0;
    virtual ~Component() = default;

    virtual void OnDrawGizmosBottom(Scene *scene) const = 0;

    virtual nlohmann::json ToJson() const = 0;

    virtual void FromJson(const nlohmann::json &json) = 0;

    // return a list of component types that this component depends on,
    // use for safe component removal (only works for same gameobject dependencies)
    virtual std::vector<std::type_index> Dependencies() { return {}; }

    // use for component creation from string
    static std::map<std::string, std::function<Component *()> > &ComponentTypeMap();

    static std::vector<std::string> &GetAvailableComponentTypes();

public:
    Logger &GetLogger() const;

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
