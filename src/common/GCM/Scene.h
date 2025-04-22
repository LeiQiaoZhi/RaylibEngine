#ifndef SCENE_H
#define SCENE_H
#include <string>

#include "GameObject.h"
#include "../editor/GameObjectHierarchyProperty.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "../context/RuntimeContext.h"
#include "../context/EditorContext.h"

#define MAX_LIGHTS 4

class Scene {
public:
    ~Scene();

    int GameObjectCount() const;

    CameraComponent *GetMainCamera() const;

    GameObject *GetGameObjectByUID(int uid) const;

    // Event Functions
    void DrawGizmos(Scene *scene) const;

    void DrawGizmosBottom(Scene *scene);

    void EditorDraw(Scene *scene) const;

    void EditorStart() const;

    void EditorUpdate() const;

    void Draw(Scene *scene) const;

    void Start() const;

    void Update() const;

    void SetRoot(GameObject *gameObject);

    GameObject *GetRoot() const {
        return root;
    }

    void FindLights();

    void SendLightInfoToShader(const Shader *shader) const;

    void SendLightInfoToModel(const Model *model) const;

    void SendEnvironmentIBLInfoToMaterial(const Material *material);

    void Save(const char *path);

    void LoadHardCodedIBLMaps();

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

    void SwitchMode();

    Color GetTint() const {
        return isPlayMode ? Fade(RED, 0.1f) : Fade(WHITE, 0.0f);
    }

    void SendEnvironmentIBLInfoToModel(const Model *model);

    std::string name;
    int selectedGameObjectUID = -1;
    Rectangle screenSpaceRect = {0, 0, 0, 0};
    GameObjectHierarchyProperty *rootFileHierarchyProperty = nullptr;

    bool isPlayMode = false;
    LuaManager luaManager;
    Logger logger;
    EditorContext editorContext = EditorContext(luaManager, logger);
    RuntimeContext runtimeContext = RuntimeContext(luaManager, logger);
    CameraComponent *editorCamera;

private:
    GameObject *root = nullptr;
    std::vector<LightComponent *> lights;

    // pbr environment maps
    // TODO: provide UI to configure these
    TextureCubemap irradianceMap = {};
    std::vector<TextureCubemap> prefilteredMap = {};
    Texture2D brdfLut = {};
};


#endif //SCENE_H
