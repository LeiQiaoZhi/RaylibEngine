//
// Created by Qiaozhi Lei on 2/6/25.
//

#ifndef HIERARCHYSUBVIEW_H
#define HIERARCHYSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../GCM/Scene.h"


class HierarchySubView {
public:
    HierarchySubView(int width, int height);

    void Render(const Scene &scene, Vector2 position = {0, 0}) const;

private:
    ISubviewRenderer *renderer_;
};


#endif //HIERARCHYSUBVIEW_H
