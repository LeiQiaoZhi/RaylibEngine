#ifndef HIERARCHYSUBVIEW_H
#define HIERARCHYSUBVIEW_H
#include "ISubviewRenderer.h"
#include "../GCM/Scene.h"


class HierarchySubView {
public:
    HierarchySubView(int width, int height);

    void Render(Scene &scene, Vector2 position = {0, 0});

    void LoadScene(Scene &scene);

private:
    ISubviewRenderer *renderer_;
    char sceneFilename[32] = "test.scene.json";
    bool editMode;
    std::string statusText;
    bool statusWarning;
};


#endif //HIERARCHYSUBVIEW_H
