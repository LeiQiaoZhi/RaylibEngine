#include "GameSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"

void GameSubView::Render(Scene &scene, const Vector2 position) const {
    const CameraComponent *camera = scene.GetMainCamera();
    if (!camera) return;
    const Camera *raylibCamera = camera->GetRaylibCamera();
    if (!raylibCamera) return;
    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Scene");

    renderer_->Begin();
    BeginMode3D(*raylibCamera);
    scene.screenSpaceRect = Rectangle{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT,
                                      renderer_->GetSize().x, renderer_->GetSize().y};
    scene.DrawGizmosBottom(&scene);
    scene.Draw(&scene);
    scene.DrawGizmos(&scene);
    EndMode3D();
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}

GameSubView::GameSubView(const int width, const int height) {
    renderer_ = new RenderTextureRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}
