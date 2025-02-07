//
// Created by Qiaozhi Lei on 2/6/25.
//

#include "HierarchySubView.h"

#include <raygui.h>

#include "ScrollPanelRenderer.h"

HierarchySubView::HierarchySubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void HierarchySubView::Render(const Scene &scene, Vector2 position) const {
    if (!scene.root) {
        return;
    }

    const Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const int totalCount = scene.GameObjectCount();
    renderer_->SetContentSize(renderer_->GetContentSize().x,
                              static_cast<float>(1 + totalCount) * textSize + 10);

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, ("Hierarchy, number of game objects: " + std::to_string(totalCount)).c_str());

    renderer_->Begin();
    const auto topLeft = renderer_->GetContentTopLeft();
    std::vector<GameObject *> toDraw;
    toDraw.push_back(scene.root);
    int i = 0;
    while (!toDraw.empty()) {
        const GameObject *current = toDraw.back();
        toDraw.pop_back();
        if (!current) continue;
        std::string spaces(current->GetSceneDepth() * 4, ' ');
        DrawText((spaces + current->GetName()).c_str(),
                 static_cast<int>(topLeft.x) + 10,
                 (i + 1) * textSize + static_cast<int>(topLeft.y),
                 textSize, textColor);
        toDraw.insert(toDraw.end(), current->GetChildren().rbegin(), current->GetChildren().rend());
        i++;
        if (i > totalCount)
            break;
    }
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
