#include "InspectorSubView.h"

#include <raygui.h>
#include <sstream>

#include "ScrollPanelRenderer.h"
#include "../Editor.h"
#include "../logger/Logger.h"

InspectorSubView::InspectorSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void InspectorSubView::Render(Scene &scene, Vector2 position) const {
    if (!scene.root) return;

    const Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const int totalCount = scene.GameObjectCount();
    const auto topLeft = renderer_->GetContentTopLeft();
    renderer_->SetContentSize(renderer_->GetContentSize().x,
                              static_cast<float>(1 + totalCount) * textSize + 10);

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Inspector");

    renderer_->Begin();
    GameObject *selectedGameObject = scene.GetGameObjectByUID(scene.selectedGameObjectUID);
    if (selectedGameObject) {
        std::ostringstream oss;
        oss << "Name: " << selectedGameObject->GetName() << " | UID: " << selectedGameObject->GetUID();
        DrawText(oss.str().c_str(),
                 topLeft.x + Editor::MediumGap(), topLeft.y + Editor::MediumGap(),
                 textSize, textColor);

        Rectangle rect = {
            topLeft.x + Editor::MediumGap() + Editor::SmallGap(), topLeft.y + Editor::MediumGap() + textSize,
            renderer_->GetContentSize().x - Editor::MediumGap() * 2 - Editor::SmallGap(),
            renderer_->GetContentSize().y - static_cast<float>(textSize)
        };

        for (auto *component: selectedGameObject->GetComponents()) {
            rect.y += 10;
            component->EditorUpdate(rect);
            rect.y += 10;
        }
    }
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
