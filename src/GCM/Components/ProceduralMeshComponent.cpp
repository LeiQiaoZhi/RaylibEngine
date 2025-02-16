#include "ProceduralMeshComponent.h"

#include <config.h>
#include <iostream>
#include <ostream>
#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "../../editor/Editor.h"
#include "../../GCM/GameObject.h"

void ProceduralMeshComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) return;

    int *meshTypePtr = reinterpret_cast<int *>(&meshType);
    GuiToggleGroup(Rectangle{rect.x, rect.y, rect.width / 2.0f, Editor::TextSize() * 1.5f}, "Cube;Plane", meshTypePtr);
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    if (meshType == MeshType::Cube) {
        sizeProperty.OnEditorGUI(rect);
        worldSizeProperty.OnEditorGUI(rect);
    } else if (meshType == MeshType::Plane) {
        planeSizeProperty.OnEditorGUI(rect);
        planeWorldSizeProperty.OnEditorGUI(rect);
    }

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Create Mesh")) {
        GenerateMesh();
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    height = rect.y - originalY;
}

float ProceduralMeshComponent::GetEditorHeight() const {
    return height;
}

void ProceduralMeshComponent::OnDraw(Scene *scene) const {
}

void ProceduralMeshComponent::OnDrawGizmos(Scene *scene) const {
}

void ProceduralMeshComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void ProceduralMeshComponent::Start() {
    modelComponent = gameObject->GetComponent<ModelComponent>();
    if (!modelComponent) {
        statusText = "Model Component not found";
        statusWarning = true;
    }
}

void ProceduralMeshComponent::Update() {
}

void ProceduralMeshComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void ProceduralMeshComponent::GenerateMesh() {
    if (!modelComponent) {
        statusText = "Model Component not found";
        statusWarning = true;
        return;
    }
    Mesh mesh{0};
    if (meshType == MeshType::Cube) {
        int sizeArray[3] = {
            static_cast<int>(cubeSize.x), static_cast<int>(cubeSize.y), static_cast<int>(cubeSize.z)
        };
        mesh = RaylibUtils::GenCubeCustomResolution(sizeArray, cubeWorldSize);
    } else if (meshType == MeshType::Plane) {
        mesh = GenMeshPlane(planeWorldSize.x, planeWorldSize.y, static_cast<int>(planeSize.x),
                            static_cast<int>(planeSize.y));
    }
    modelComponent->SetModelFromMesh(mesh);
}

nlohmann::json ProceduralMeshComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "ProceduralMeshComponent";
    j["meshType"] = static_cast<int>(meshType);
    j["cubeSize"] = {cubeSize.x, cubeSize.y, cubeSize.z};
    j["cubeWorldSize"] = {cubeWorldSize.x, cubeWorldSize.y, cubeWorldSize.z};
    j["planeSize"] = {planeSize.x, planeSize.y};
    j["planeWorldSize"] = {planeWorldSize.x, planeWorldSize.y};
    return j;
}

void ProceduralMeshComponent::FromJson(const nlohmann::json &json) {
    meshType = static_cast<MeshType>(json["meshType"]);
    cubeSize = {json["cubeSize"][0], json["cubeSize"][1], json["cubeSize"][2]};
    cubeWorldSize = {json["cubeWorldSize"][0], json["cubeWorldSize"][1], json["cubeWorldSize"][2]};
    planeSize = {json["planeSize"][0], json["planeSize"][1]};
    planeWorldSize = {json["planeWorldSize"][0], json["planeWorldSize"][1]};
}

