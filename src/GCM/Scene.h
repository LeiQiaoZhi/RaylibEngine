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

    GameObject *GetGameObjectByUID(int uid) const;

    void DrawGizmos(Scene *scene) const;

    void Draw(Scene * scene);

    std::string name;
    GameObject *root = nullptr;
    int selectedGameObjectUID = -1;
};


#endif //SCENE_H
