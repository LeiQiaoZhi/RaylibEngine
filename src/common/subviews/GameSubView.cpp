#include "GameSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"

void GameSubView::Render(Scene &scene, const Vector2 position) {
    CameraComponent *camera = scene.GetMainCamera();
    if (!camera) return;
    const Camera *raylibCamera = camera->GetRaylibCamera();
    if (!raylibCamera) return;

    const char *title = scene.isPlayMode ? "Game" : "Scene";
    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, title);

    // Play mode button
    const int icon = scene.isPlayMode ? ICON_PLAYER_STOP : ICON_PLAYER_PLAY;
    const float buttonWidth = Editor::TextSize() + 2 * Editor::SmallGap();
    float x = position.x + (renderer_->GetSize().x - buttonWidth) / 2;
    if (GuiButton({x, position.y, buttonWidth, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT},
                  GuiIconText(icon, ""))) {
        scene.SwitchMode();
    }

    // Edit mode camera settings
    x += buttonWidth + Editor::SmallGap();
    const float cameraSettingsButtonWidth = Editor::TextWidth("Camera Settings") + 2 * Editor::SmallGap();
    if (!scene.isPlayMode && GuiButton({
                      x, position.y, cameraSettingsButtonWidth,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                  }, "Camera Settings")) {
        showCameraSettings = !showCameraSettings;
    }

    renderer_->Begin();
    BeginMode3D(*raylibCamera);
    scene.screenSpaceRect = Rectangle{
        position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT,
        renderer_->GetSize().x, renderer_->GetSize().y
    };
    scene.DrawBackground();
    scene.DrawGizmosBottom(&scene);
    if (scene.isPlayMode) {
        scene.Draw(&scene);
    } else {
        scene.EditorDraw(&scene);
    }
    scene.DrawGizmos(&scene);
    EndMode3D();
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);

    if (!scene.isPlayMode && showCameraSettings) {
        Rectangle rect = {
            x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT,
            position.x + renderer_->GetSize().x - x, camera->GetEditorHeight()
        };
        DrawRectangleRec(rect, Editor::BackgroundColor());
        camera->OnEditorGUI(rect);
    }
}

GameSubView::GameSubView(const int width, const int height) {
    renderer_ = new RenderTextureRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}
