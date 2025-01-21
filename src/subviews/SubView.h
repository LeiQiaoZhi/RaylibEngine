#ifndef SUBVIEW_H
#define SUBVIEW_H
#include "raylib.h"


// TODO: support for window size changes

class SubView {
public:
    SubView(int width, int height);

    virtual ~SubView();

    void Show(Vector2 position = {0, 0}, Color tint = WHITE) const;

protected:
    int width_;
    int height_;
    RenderTexture render_texture_{};
    Rectangle view_rect_{};
};


#endif //SUBVIEW_H
