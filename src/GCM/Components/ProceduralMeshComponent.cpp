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

    sizeProperty.OnEditorGUI(rect);
    worldSizeProperty.OnEditorGUI(rect);

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Create Mesh")) {
        if (!modelComponent) {
            warningText = "Model Component not found";
            return;
        }
        int sizeArray[3] = {static_cast<int>(cubeSize.x), static_cast<int>(cubeSize.y), static_cast<int>(cubeSize.z)};
        const Mesh mesh = RaylibUtils::GenCubeCustomResolution(sizeArray, cubeWorldSize);
        modelComponent->SetModelFromMesh(mesh);
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // warning
    if (!warningText.empty()) {
        const char *text = GuiIconText(ICON_WARNING, warningText.c_str());
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, text);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

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
        warningText = "Model Component not found";
    }
}

void ProceduralMeshComponent::Update() {
}

void ProceduralMeshComponent::OnDrawGizmosBottom(Scene *scene) const {
}
