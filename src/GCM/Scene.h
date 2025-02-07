#ifndef SCENE_H
#define SCENE_H
#include <string>

#include "GameObject.h"
#include "Components/CameraComponent.h"


class Scene {
public:
    int GameObjectCount() const;
    CameraComponent *GetMainCamera() const;
    void UpdateComponents() const;

    std::string name;
    GameObject *root = nullptr;
};


#endif //SCENE_H
