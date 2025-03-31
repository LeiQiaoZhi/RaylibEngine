#include "ShaderCompilationSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"
#include "CodeGeneration.h"

void ShaderCompilationSubView::Render(Vector2 position, Context &context) {
    const auto topLeft = renderer_->GetContentTopLeft();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Compilation");

    Rectangle rect = {
        topLeft.x + Editor::SmallGap(), topLeft.y + Editor::SmallGap(),
        renderer_->GetContentSize().x - Editor::SmallGap() * 2,
        renderer_->GetContentSize().y - Editor::SmallGap() * 2
    };

    renderer_->Begin();

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Compile")) {
        nodes = CodeGeneration::FilterNodes(context.FinalNode());
        orderedNodes = CodeGeneration::TopologicalSort(nodes);
        const std::string prefix = CodeGeneration::GetPrefix();
        const std::string functions = CodeGeneration::GetFunctions(context.FinalNode());
        const std::string main = CodeGeneration::GetMain(orderedNodes);
        context.SendShaderCode(prefix + functions + main);
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // nodes
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, TextFormat("Nodes: %d", nodes.size()));
    rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
    for (auto &node: nodes) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, node->name.c_str());
        rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
    }
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
             TextFormat("Ordered Nodes: %d", orderedNodes.size()));
    rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
    for (auto &node: orderedNodes) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, node->name.c_str());
        rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
    }

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
