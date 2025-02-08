#ifndef ASSETSSUBVIEW_H
#define ASSETSSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../GCM/Scene.h"


class AssetsSubView {
public:
    AssetsSubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0}) const;

private:
    ISubviewRenderer *renderer_;
};


#endif //ASSETSSUBVIEW_H
