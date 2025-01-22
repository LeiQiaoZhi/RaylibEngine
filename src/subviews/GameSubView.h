#ifndef GAMESUBVIEW_H
#define GAMESUBVIEW_H
#include "RenderTextureRenderer.h"

class GameSubView {
public:
    GameSubView(int width, int height);

    void Render(const Camera &camera, Vector2 position = {0, 0}) const;

private:
    ISubviewRenderer *renderer_;
};


#endif //GAMESUBVIEW_H
