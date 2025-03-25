#ifndef GAMESUBVIEW_H
#define GAMESUBVIEW_H
#include "RenderTextureRenderer.h"
#include "../GCM/Scene.h"

class GameSubView {
public:
    GameSubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0});

private:
    ISubviewRenderer *renderer_;
    bool showCameraSettings = false;
};


#endif //GAMESUBVIEW_H
