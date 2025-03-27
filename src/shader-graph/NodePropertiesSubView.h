#ifndef NodePropertiesSubView_H
#define NodePropertiesSubView_H
#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/GCM/Scene.h"
#include "../common/editor/FileHierarchyProperty.h"
#include "nodes/Node.h"

class NodePropertiesSubView {
public:
    NodePropertiesSubView(int width, int height, std::vector<Node> &nodes)
        : nodes(nodes) {
        renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
    }

    void Render(Vector2 position = {0, 0});

    Vector2 GetSize() const {
        return renderer_->GetSize();
    }

private:
    ISubviewRenderer *renderer_;
    std::vector<Node> &nodes;
};


#endif //NodePropertiesSubView_H
