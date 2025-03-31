#ifndef ShaderCompilationSubView_H
#define ShaderCompilationSubView_H
#include "../common/editor/DropdownProperty.h"
#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/GCM/Scene.h"
#include "../common/editor/FileHierarchyProperty.h"
#include "nodes/Node.h"

class ShaderCompilationSubView {
public:
    ShaderCompilationSubView(int width, int height) {
        renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
    }

    void Render(Vector2 position, Context &context);

    Vector2 GetSize() const {
        return renderer_->GetSize();
    }

private:
    ISubviewRenderer *renderer_;
    std::set<Node *> nodes;

    // topologically sort
    std::list<Node *> orderedNodes;
    std::set<Node *> visited;
};


#endif //ShaderCompilationSubView_H
