#ifndef GAMEOBJECTHIERARCHYPROPERTY_H
#define GAMEOBJECTHIERARCHYPROPERTY_H
#include <map>
#include "../utils/RaylibUtils.h"

#include "raylib.h"


class GameObject;

class GameObjectHierarchyProperty {
public:
    explicit GameObjectHierarchyProperty(GameObject *gameObject, const bool folded = true)
        : gameObject(gameObject), folded(folded) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const {
        return folded;
    }

private:
    GameObject *gameObject;
    bool folded;
    int height;
    std::map<int, std::unique_ptr<GameObjectHierarchyProperty> > children;
    bool showMessageBox = false;
};


#endif //FILEHIERARCHYPROPERTY_H
