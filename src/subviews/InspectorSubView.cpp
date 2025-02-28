#include "InspectorSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "ScrollPanelRenderer.h"
#include "../editor/Editor.h"
#include "../logger/Logger.h"

InspectorSubView::InspectorSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void InspectorSubView::Render(Scene &scene, Vector2 position) {
    if (!scene.GetRoot()) return;
    GameObject *selectedGameObject = scene.GetGameObjectByUID(scene.selectedGameObjectUID);
    const auto topLeft = renderer_->GetContentTopLeft();
    Rectangle rect = {
        topLeft.x + Editor::MediumGap() + Editor::SmallGap(), topLeft.y + Editor::MediumGap(),
        renderer_->GetContentSize().x - Editor::MediumGap() * 2 - Editor::SmallGap(),
        renderer_->GetContentSize().y - Editor::MediumGap()
    };
    float height = rect.y - topLeft.y + Editor::MediumGap();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Inspector");

    renderer_->Begin();
    if (selectedGameObject) {
        const float labelWidth = Editor::TextWidth("Name: ") + Editor::MediumGap();
        const std::string uidString = "UID: " + std::to_string(selectedGameObject->GetUID());
        const float uidWidth = Editor::TextWidth(uidString.c_str()) + Editor::MediumGap();
        GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "Name: ");
        Editor::BeginIndent(rect, labelWidth);
        strncpy(nameBuffer, selectedGameObject->name.c_str(), 255);
        nameBuffer[255] = '\0';  // Ensure null termination
        if (GuiTextBox({
                           rect.x, rect.y, rect.width - uidWidth - Editor::SmallGap(),
                           Editor::TextSize() * 1.0f
                       },
                       &selectedGameObject->name[0], 256, nameEditMode)) {
            nameEditMode = !nameEditMode;
            selectedGameObject->name = nameBuffer;
        }
        GuiLabel({rect.x + rect.width - uidWidth, rect.y, uidWidth, Editor::TextSize() * 1.0f}, uidString.c_str());
        Editor::EndIndent(rect, labelWidth);
        rect.y += Editor::TextSize() + Editor::SmallGap();
        height += Editor::TextSize() + Editor::SmallGap();

        // Components
        std::vector<Component *> componentsToRemove;
        for (auto *component: selectedGameObject->GetComponents()) {
            GuiLine({rect.x, rect.y, rect.width, Editor::LargeGap() * 1.0f}, nullptr);
            rect.y += Editor::LargeGap();
            component->OnEditorGUI(rect);

            if (!component->removed) // in case component is removed during this iteration
                height += component->GetEditorHeight() + Editor::LargeGap();
            else
                componentsToRemove.push_back(component);
        }

        for (auto *component: componentsToRemove) {
            selectedGameObject->RemoveComponent(component);
        }

        GuiLine({rect.x, rect.y, rect.width, Editor::LargeGap() * 1.0f}, nullptr);
        rect.y += Editor::LargeGap();
        addComponentProperty.SetTargetGameObject(selectedGameObject);
        addComponentProperty.OnEditorGUI(rect);
        height += addComponentProperty.GetEditorHeight() + Editor::LargeGap();
    }
    renderer_->End();
    renderer_->SetContentSize(renderer_->GetContentSize().x, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
