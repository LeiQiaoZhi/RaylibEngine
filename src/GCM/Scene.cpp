#include "Scene.h"
#include "GameObject.h"

int Scene::GameObjectCount() const {
    return root->RecursiveChildCount() + 1;
}

CameraComponent *Scene::GetMainCamera() const {
    return root->GetComponentInChildren<CameraComponent>();
}

void Scene::UpdateComponents() const {
    root->UpdateComponents();
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

void Scene::Draw(Scene *scene) {
    root->Draw(scene);
}

void Scene::DrawGizmosBottom(Scene *scene) {
    root->DrawGizmosBottom(scene);
}

void Scene::StartComponents() const {
    root->StartComponents();
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
        const Shader *shader = &model->materials[i].shader;
        SendLightInfoToShader(shader);
    }
}

void Scene::Save(const char *path) {
    nlohmann::json j = {};
    j["name"] = name;
    j["root"] = root->ToJson();
    j["selectedGameObjectUID"] = selectedGameObjectUID;
    j["saveTime"] = time(nullptr);
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

    // initialize scene
    StartComponents();
    FindLights();
}
