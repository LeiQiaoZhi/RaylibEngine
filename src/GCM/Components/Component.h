#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
public:
    virtual void EditorUpdate() = 0;

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
