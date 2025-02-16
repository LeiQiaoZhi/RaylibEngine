#ifndef INSPECTORSUBVIEW_H
#define INSPECTORSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../editor/AddComponentProperty.h"
#include "../editor/DropdownProperty.h"
#include "../GCM/Scene.h"


class InspectorSubView {
public:
    InspectorSubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0});

private:
    ISubviewRenderer *renderer_;
    AddComponentProperty addComponentProperty = AddComponentProperty(nullptr);
};


#endif //INSPECTORSUBVIEW_H
