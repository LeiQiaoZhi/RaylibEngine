//
// Created by Qiaozhi Lei on 1/21/25.
//

#include "RenderTextureRenderer.h"

#include <raygui.h>

RenderTextureRenderer::RenderTextureRenderer(int width, int height) : width_(width), height_(height) {
    renderTexture_ = LoadRenderTexture(width_, height_);
    viewRect_ = {
        0, 0,
        static_cast<float>(renderTexture_.texture.width),
        // Invert the height to match the coordinate system
        static_cast<float>(-renderTexture_.texture.height)
    };
}

RenderTextureRenderer::~RenderTextureRenderer() {
    UnloadRenderTexture(renderTexture_);
}

void RenderTextureRenderer::Begin() {
    BeginTextureMode(renderTexture_);
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
}

void RenderTextureRenderer::End() {
    EndTextureMode();
}

void RenderTextureRenderer::Show(const Vector2 vector2, const Color tint) {
    DrawTextureRec(renderTexture_.texture, viewRect_, vector2, tint);
}
