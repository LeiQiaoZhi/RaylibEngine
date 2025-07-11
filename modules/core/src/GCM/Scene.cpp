#include "Scene.h"
#include "GameObject.h"
#include "rlgl.h"

Scene::~Scene() {
    delete root;
    delete editorCamera->gameObject;
}

int Scene::GameObjectCount() const {
    return root->RecursiveChildCount() + 1;
}

CameraComponent *Scene::GetMainCamera() const {
    if (isPlayMode)
        return root->GetComponentInChildren<CameraComponent>();
    else
        return editorCamera;
}


GameObject *Scene::GetGameObjectByUID(const int uid) const {
    if (uid < 0)
        return nullptr;

    return root->GetGameObjectByUID(uid);
}

void Scene::DrawGizmos(Scene *scene) const {
    root->DrawGizmos(scene);

    // draw gizmos for selected game object
    if (selectedGameObjectUID != -1) {
        const auto *selectedGameObject = GetGameObjectByUID(selectedGameObjectUID);
        if (selectedGameObject) {
            selectedGameObject->DrawGizmosSelected(scene);
        }
    }
}


void Scene::DrawGizmosBottom(Scene *scene) {
    root->DrawGizmosBottom(scene);
}

void Scene::EditorDraw(Scene *scene) const {
    root->EditorDraw(scene);
    editorCamera->OnEditorDraw(scene);
}

void Scene::EditorUpdate() const {
    root->EditorUpdate();
    editorCamera->EditorUpdate();
}

void Scene::EditorStart() const {
    root->EditorStart();
    editorCamera->EditorStart();
}

void Scene::Draw(Scene *scene) const {
    root->Draw(scene);
}

void Scene::Update() const {
    root->Update();
}

void Scene::Start() const {
    root->Start();
}

void Scene::SetRoot(GameObject *gameObject) {
    root = gameObject;
    root->scene = this;
}

void Scene::FindLights() {
    lights.clear();
    lights = root->GetComponentsInChildren<LightComponent>();
}

void Scene::SendLightInfoToShader(const Shader *shader) const {
    const int lightsCountLoc = GetShaderLocation(*shader, "lightsCount");
    const int lightsCount = std::min(static_cast<int>(lights.size()), MAX_LIGHTS);
    SetShaderValue(*shader, lightsCountLoc, &lightsCount, SHADER_UNIFORM_INT);

    const Camera camera = *GetMainCamera()->GetRaylibCamera();
    const float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
    SetShaderValue(*shader, shader->locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    for (int i = 0; i < lightsCount; i++) {
        const LightComponent *light = lights[i];
        if (!light || !light->enabled) continue;

        const int typeLoc = GetShaderLocation(*shader, TextFormat("lights[%i].type", i));
        const int intensityLoc = GetShaderLocation(*shader, TextFormat("lights[%i].intensity", i));
        const int rangeLoc = GetShaderLocation(*shader, TextFormat("lights[%i].range", i));
        const int positionLoc = GetShaderLocation(*shader, TextFormat("lights[%i].position", i));
        const int directionLoc = GetShaderLocation(*shader, TextFormat("lights[%i].direction", i));
        const int colorLoc = GetShaderLocation(*shader, TextFormat("lights[%i].color", i));

        const Vector3 position = light->gameObject->GetTransform()->GetWorldPosition();
        const float positionArray[3] = {position.x, position.y, position.z};
        const Vector3 direction = light->gameObject->GetTransform()->GetWorldForward();
        const float directionArray[3] = {direction.x, direction.y, direction.z};
        const float colorArray[3] = {
            static_cast<float>(light->color.r) / 255.0f,
            static_cast<float>(light->color.g) / 255.0f,
            static_cast<float>(light->color.b) / 255.0f,
        };

        SetShaderValue(*shader, typeLoc, &light->lightType, SHADER_UNIFORM_INT);
        SetShaderValue(*shader, intensityLoc, &light->intensity, SHADER_UNIFORM_FLOAT);
        SetShaderValue(*shader, rangeLoc, &light->range, SHADER_UNIFORM_FLOAT);
        SetShaderValue(*shader, positionLoc, positionArray, SHADER_UNIFORM_VEC3);
        SetShaderValue(*shader, directionLoc, directionArray, SHADER_UNIFORM_VEC3);
        SetShaderValue(*shader, colorLoc, colorArray, SHADER_UNIFORM_VEC3);
    }
}

void Scene::SendLightInfoToModel(const Model *model) const {
    for (int i = 0; i < model->materialCount; i++) {
        const Shader &shader = model->materials[i].shader;
        SendLightInfoToShader(&shader);
    }
}

void Scene::SendEnvironmentIBLInfoToMaterial(const Material *material) {
    if (!IsTextureValid(irradianceMap)) {
        logger.Log("Irradiance map not ready for material", LogLevel::Error);
    } else {
        LoadHardCodedIBLMaps();
        material->maps[MATERIAL_MAP_IRRADIANCE].texture = irradianceMap;
        material->maps[MATERIAL_MAP_IRRADIANCE].color = WHITE;

        material->maps[MATERIAL_MAP_PREFILTER].texture = prefilteredMap;
        material->maps[MATERIAL_MAP_PREFILTER].color = WHITE;

        material->maps[MATERIAL_MAP_BRDF].texture = brdfLut;
        material->maps[MATERIAL_MAP_BRDF].color = WHITE;
    }
}

void Scene::SendEnvironmentIBLInfoToModel(const Model *model) {
    for (int i = 0; i < model->materialCount; i++) {
        const Material &material = model->materials[i];
        const Shader &shader = model->materials[i].shader;

        if (!IsTextureValid(irradianceMap)) {
            logger.Log("Failed to load irradiance map", LogLevel::Error);
        } else {
            logger.Log("Sending environment IBL info to model");
            material.maps[MATERIAL_MAP_IRRADIANCE].texture = irradianceMap;
        }
    }
}

void Scene::LoadHardCodedIBLMaps() {
    const Image irradianceImage = LoadImage(INTERNAL_ASSET_DIR "/textures/irradiance.png");
    irradianceMap = LoadTextureCubemap(irradianceImage, CUBEMAP_LAYOUT_AUTO_DETECT);
    if (!IsTextureValid(irradianceMap)) {
        logger.Log("Failed to load irradiance map", LogLevel::Error);
    } else {
        logger.Log("Irradiance map loaded");
    }
    const Image prefilterImage = LoadImage(INTERNAL_ASSET_DIR "/textures/prefilter.dds");
    prefilteredMap = LoadTextureCubemap(prefilterImage, CUBEMAP_LAYOUT_AUTO_DETECT);
    if (!IsTextureValid(prefilteredMap)) {
        logger.Log("Failed to load prefilter map", LogLevel::Error);
    } else {
        logger.Log("Prefilter map loaded");
    }
    brdfLut = LoadTexture(INTERNAL_ASSET_DIR "/textures/brdfLUT.png");
    if (!IsTextureValid(brdfLut)) {
        logger.Log("Failed to load brdfLUT", LogLevel::Error);
    } else {
        logger.Log("brdfLUT loaded");
    }
}

void Scene::Save(const char *path) {
    nlohmann::json j = {};
    j["name"] = name;
    j["root"] = root->ToJson();
    j["selectedGameObjectUID"] = selectedGameObjectUID;
    j["saveTime"] = time(nullptr);
    j["editorCamera"] = editorCamera->ToJson();
    JsonUtils::JsonToFile(j, path);
}


void Scene::Load(const char *path) {
    // unload current scene
    delete root;

    // load new scene from json
    nlohmann::json j = JsonUtils::JsonFromFile(path);
    name = j["name"];
    selectedGameObjectUID = j["selectedGameObjectUID"];
    root = new GameObject(j["root"], this);

    if (editorCamera == nullptr) {
        editorCamera = new CameraComponent();
        // hidden obj to hold editor camera
        GameObject *hiddenObj = new GameObject("Editor Camera", Utils::GenerateUID("Editor Camera"));
        hiddenObj->scene = this;
        hiddenObj->AddComponent(editorCamera);
    }
    if (j.contains("editorCamera"))
        editorCamera->FromJson(j["editorCamera"]);
    // initialize scene
    FindLights();
    LoadHardCodedIBLMaps();

    rootFileHierarchyProperty = new GameObjectHierarchyProperty(root, false);

    // start when all objs are loaded
    Start();
}

void Scene::DrawBackground() {
    GetMainCamera()->DrawBackground();
}

Ray Scene::GetMouseScreenToWorldRay() {
    const Vector2 mousePosition = GlobalToLocalScreenSpace(GetMousePosition());
    const float width = screenSpaceRect.width;
    const float height = screenSpaceRect.height;
    const Ray ray = GetScreenToWorldRayEx(mousePosition, *GetMainCamera()->GetRaylibCamera(), width, height);
    return ray;
}

void Scene::SwitchMode() {
    isPlayMode = !isPlayMode;
}
