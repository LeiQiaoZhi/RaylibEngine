#ifndef INSPECTORSUBVIEW_H
#define INSPECTORSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../GCM/Scene.h"


class InspectorSubView {
public:
    InspectorSubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0}) const;

private:
    ISubviewRenderer *renderer_;
};


#endif //INSPECTORSUBVIEW_H
