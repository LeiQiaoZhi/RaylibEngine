//
// Created by Qiaozhi Lei on 1/20/25.
//

#include "GameSubView.h"

#include <raygui.h>

void GameSubView::Begin(const Camera &camera) const {
    BeginTextureMode(render_texture_);
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    BeginMode3D(camera);
    DrawGrid(20, 20);
    EndMode3D();
    EndTextureMode();
}
