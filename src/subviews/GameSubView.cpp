#include "GameSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"

void GameSubView::Render(const Camera &camera, const Vector2 position) const {
    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Scene");

    renderer_->Begin();
    BeginMode3D(camera);
    DrawGrid(20, 20);
    EndMode3D();
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}

GameSubView::GameSubView(const int width, const int height) {
    renderer_ = new RenderTextureRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}
