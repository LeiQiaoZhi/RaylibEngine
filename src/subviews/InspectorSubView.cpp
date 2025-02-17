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
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const auto topLeft = renderer_->GetContentTopLeft();
    Rectangle rect = {
        topLeft.x + Editor::MediumGap() + Editor::SmallGap(), topLeft.y + Editor::MediumGap() + textSize,
        renderer_->GetContentSize().x - Editor::MediumGap() * 2 - Editor::SmallGap(),
        renderer_->GetContentSize().y - Editor::MediumGap() - textSize
    };
    float height = rect.y - topLeft.y + Editor::MediumGap();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Inspector");

    renderer_->Begin();
    if (selectedGameObject) {
        std::ostringstream oss;
        oss << "Name: " << selectedGameObject->GetName() << " | UID: " << selectedGameObject->GetUID();
        GuiLabel({
                     topLeft.x + Editor::MediumGap(), topLeft.y + Editor::MediumGap(),
                     renderer_->GetContentSize().x, textSize * 1.0f
                 }, oss.str().c_str());
        height += textSize + Editor::MediumGap();

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
        height += addComponentProperty.GetEditorHeight();
    }
    renderer_->End();
    renderer_->SetContentSize(renderer_->GetContentSize().x, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
