#ifndef SCENE_H
#define SCENE_H
#include <string>

#include "GameObject.h"


class Scene {
public:
    int GameObjectCount() const;

    std::string name;
    GameObject *root = nullptr;
};


#endif //SCENE_H
