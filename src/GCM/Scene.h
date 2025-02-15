#ifndef SCENE_H
#define SCENE_H
#include <string>

#include "GameObject.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"

#define MAX_LIGHTS 4

class Scene {
public:
    int GameObjectCount() const;

    CameraComponent *GetMainCamera() const;

    void UpdateComponents() const;

    GameObject *GetGameObjectByUID(int uid) const;

    void DrawGizmos(Scene *scene) const;

    void Draw(Scene *scene);

    void DrawGizmosBottom(Scene *scene);

    void StartComponents() const;

    void SetRoot(GameObject *gameObject);

    GameObject *GetRoot() const {
        return root;
    }

    void FindLights();

    void SendLightInfoToShader(const Shader *shader) const;

    void SendLightInfoToModel(const Model *model) const;

    std::string name;
    int selectedGameObjectUID = -1;

private:
    GameObject *root = nullptr;
    std::vector<LightComponent *> lights;
};


#endif //SCENE_H
