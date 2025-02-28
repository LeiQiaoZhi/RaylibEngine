#ifndef SCENE_H
#define SCENE_H
#include <string>

#include "GameObject.h"
#include "../editor/GameObjectHierarchyProperty.h"
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

    void Save(const char *path);

    void Load(const char *data);

    Vector2 GlobalToLocalScreenSpace(const Vector2 &position) const {
        return Vector2{position.x - screenSpaceRect.x, position.y - screenSpaceRect.y};
    }

    Vector2 LocalToGlobalScreenSpace(const Vector2 &position) const {
        return Vector2{position.x + screenSpaceRect.x, position.y + screenSpaceRect.y};
    }

    void DrawBackground();

    Ray GetMouseScreenToWorldRay();

    bool IsInsideScenePanel(const Vector2 point) const {
        return CheckCollisionPointRec(point, screenSpaceRect);
    }

    std::string name;
    int selectedGameObjectUID = -1;
    Rectangle screenSpaceRect = {0, 0, 0, 0};
    GameObjectHierarchyProperty* rootFileHierarchyProperty = nullptr;

private:
    GameObject *root = nullptr;
    std::vector<LightComponent *> lights;
};


#endif //SCENE_H
