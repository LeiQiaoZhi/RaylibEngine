#include "HierarchySubView.h"

#include <raygui.h>

#include "ScrollPanelRenderer.h"
#include "../logger/Logger.h"
#include "../Editor.h"

HierarchySubView::HierarchySubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void HierarchySubView::Render(Scene &scene, Vector2 position) const {
    if (!scene.root) {
        return;
    }

    const Color normalTextColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    const Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
    const Color pressed = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_PRESSED));
    const Color focused = GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED));
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
        const Rectangle rect = {
            topLeft.x + 10.0f + current->GetSceneDepth() * 20,
            (i + 1) * textSize + topLeft.y,
            renderer_->GetContentSize().x - 10, static_cast<float>(textSize)
        };
        Color buttonColor = bgColor;
        Color textColor = normalTextColor;
        if (scene.selectedGameObjectUID == current->GetUID()) {
            buttonColor = pressed;
            textColor = bgColor;
        }
        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            buttonColor = Editor::DisabledColor(); // hover
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                scene.selectedGameObjectUID = current->GetUID();
            }
        }
        DrawRectangleRec(rect, buttonColor);
        DrawText(current->GetName(), rect.x, rect.y, textSize, textColor);


        toDraw.insert(toDraw.end(), current->GetChildren().rbegin(), current->GetChildren().rend());
        i++;
        if (i > totalCount)
            break;
    }
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
