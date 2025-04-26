#ifndef ShaderCompilationSubView_H
#define ShaderCompilationSubView_H
#include "CodeGeneration.h"
#include "editor/DropdownProperty.h"
#include "subviews/ScrollPanelRenderer.h"
#include "GCM/Scene.h"
#include "editor/FileHierarchyProperty.h"
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

    // export
    CodeGeneration::ShaderFileType exportType = CodeGeneration::ShaderFileType::GLSL;
    std::string exportName = "shader";
    bool exportNameEditMode = false;
    std::string statusText;
    bool statusWarning = false;

    // topologically sort
    std::list<Node *> orderedNodes;
    std::set<Node *> visited;

    FoldoutProperty nodesFoldout = FoldoutProperty("Nodes", true);
    FoldoutProperty orderedNodesFoldout = FoldoutProperty("Ordered Nodes", true);
};


#endif //ShaderCompilationSubView_H
