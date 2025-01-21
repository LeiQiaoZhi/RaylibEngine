#include "SubView.h"

SubView::SubView(const int width, const int height) : width_(width), height_(height) {
    render_texture_ = LoadRenderTexture(800, 800);
    view_rect_ = {
        0, 0,
        static_cast<float>(render_texture_.texture.width),
        // Invert the height to match the coordinate system
        static_cast<float>(-render_texture_.texture.height)
    };
}

SubView::~SubView() {
    UnloadRenderTexture(render_texture_);
}

void SubView::Show(const Vector2 position, const Color tint) const {
    DrawTextureRec(render_texture_.texture, view_rect_, position, tint);
}
