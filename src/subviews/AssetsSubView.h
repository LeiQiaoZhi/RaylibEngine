#ifndef ASSETSSUBVIEW_H
#define ASSETSSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../GCM/Scene.h"
#include "../editor/FileHierarchyProperty.h"


class AssetsSubView {
public:
    AssetsSubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0});

private:
    ISubviewRenderer *renderer_;
    FileHierarchyProperty rootFileHierarchyProperty;
    bool pathEditMode;
    char path[256] = ASSET_DIR;
    std::string status;
};


#endif //ASSETSSUBVIEW_H
