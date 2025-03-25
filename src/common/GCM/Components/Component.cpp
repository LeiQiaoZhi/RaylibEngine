//
// Created by Qiaozhi Lei on 2/7/25.
//

#include "Component.h"

#include "CameraComponent.h"
#include "DebugGridBoxComponent.h"
#include "DebugGridComponent.h"
#include "JelloComponent.h"
#include "LightComponent.h"
#include "RigidbodyComponent.h"
#include "ScriptComponent.h"
#include "SphereColliderComponent.h"
#include "TransformComponent.h"

std::map<std::string, std::function<Component *()> > &Component::ComponentTypeMap() {
    static std::map<std::string, std::function<Component *()> > map = {
        {"CameraComponent", []() { return new CameraComponent(); }},
        {"DebugGridBoxComponent", []() { return new DebugGridBoxComponent(); }},
        {"DebugGridComponent", []() { return new DebugGridComponent(); }},
        {"JelloComponent", []() { return new JelloComponent(); }},
        {"LightComponent", []() { return new LightComponent(); }},
        {"ModelComponent", []() { return new ModelComponent(); }},
        {"ProceduralMeshComponent", []() { return new ProceduralMeshComponent(); }},
        {"TransformComponent", []() { return new TransformComponent(); }},
        {"ScriptComponent", []() { return new ScriptComponent(); }},
        {"SphereColliderComponent", []() { return new SphereColliderComponent(); }},
        {"RigidbodyComponent", []() { return new RigidbodyComponent(); }},
    };
    return map;
}

std::vector<std::string> &Component::GetAvailableComponentTypes() {
    static std::vector<std::string> types;
    if (types.empty()) {
        const auto &map = ComponentTypeMap();
        for (const auto &pair: map) {
            types.push_back(pair.first);
        }
    }
    return types;
}

Logger &Component::GetLogger() const {
    return gameObject->scene->runtimeContext.logger;
}
