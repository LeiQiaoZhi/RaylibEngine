#ifndef PreviewSubView_H
#define PreviewSubView_H
#include "subviews/RenderTextureRenderer.h"
#include "GCM/Scene.h"
#include "editor/FileHierarchyProperty.h"
#include "nodes/Node.h"

class PreviewSubView {
public:
    PreviewSubView(int width, int height) {
        renderer_ = new RenderTextureRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);

        camera.target = (Vector3){0.0f, 0.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.position = (Vector3){0.0f, 1.5f, 0.0f};
        camera.projection = CAMERA_PERSPECTIVE;
        camera.up = (Vector3){0.0f, 0.0f, -1.0f};

        model = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1));
    }

    void Render(Vector2 position = {0, 0});

    void Update(Context &context);

    Vector2 GetSize() const {
        return renderer_->GetSize();
    }

private:
    ISubviewRenderer *renderer_;
    Camera camera;
    CameraMode cameraMode = CAMERA_THIRD_PERSON;
    Model model;
    std::string statusText;
    bool statusWarning;
};


#endif //PreviewSubView_H
