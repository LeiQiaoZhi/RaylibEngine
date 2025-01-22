#include "GameSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"

void GameSubView::Render(const Camera &camera, const Vector2 position) const {
    renderer_->Begin();
    BeginMode3D(camera);
    DrawGrid(20, 20);
    EndMode3D();
    renderer_->End();
    renderer_->Show(position, WHITE);
}

GameSubView::GameSubView(const int width, const int height) {
    renderer_ = new RenderTextureRenderer(width, height);
}
