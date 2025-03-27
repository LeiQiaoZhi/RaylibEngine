#include "PreviewSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"


void PreviewSubView::Render(Vector2 position) {
    const auto topLeft = renderer_->GetContentTopLeft();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Preview");

    renderer_->Begin();
    BeginMode3D(camera);

    ClearBackground(Editor::BackgroundColor());
    // DrawModel(model, {0, 0, 0}, 1.0f, WHITE);
    DrawPlane({0, 0, 0}, {1, 1}, WHITE);

    EndMode3D();
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}

void PreviewSubView::Update() {
    // UpdateCamera(&camera, cameraMode);
}
