#ifndef ADDCOMPONENTPROPERTY_H
#define ADDCOMPONENTPROPERTY_H
#include <sstream>
#include <string>

#include "DropdownProperty.h"
#include "raylib.h"
#include "../utils/RaylibUtils.h"
#include "../GCM/GameObject.h"


class AddComponentProperty {
public:
    explicit AddComponentProperty(GameObject *gameObject) : gameObject(gameObject) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

    void SetTargetGameObject(GameObject *gameObject) {
        this->gameObject = gameObject;
    }

private:
    // States
    GameObject * gameObject;

    // UI
    DropdownProperty dropdownProperty = DropdownProperty(Component::GetAvailableComponentTypes(), 0);
    float height = 0;
    std::string statusText = "";
    bool statusWarning = false;
};


#endif //ADDCOMPONENTPROPERTY_H
