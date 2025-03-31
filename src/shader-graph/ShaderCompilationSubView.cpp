#include "ShaderCompilationSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"
#include "CodeGeneration.h"


void ShaderCompilationSubView::FilterNodes(const Context &context) {
    nodes.clear();
    // 1. DFS to find all nodes reachable from final node
    std::list<Node *> toExplore = {context.finalNode};
    while (!toExplore.empty()) {
        Node *node = toExplore.front();
        toExplore.pop_front();
        nodes.insert(node);

        std::set<Node *> neighbours = node->GetNeighboursFromInputs();
        toExplore.insert(toExplore.end(), neighbours.begin(), neighbours.end());
    }
}

void ShaderCompilationSubView::TopologicalSortVisit(Node *node) {
    visited.insert(node);
    std::set<Node *> neighbours = node->GetNeighboursFromOutputs();
    for (auto &neighbour: neighbours) {
        if (visited.find(neighbour) == visited.end() && nodes.find(neighbour) != nodes.end()) {
            TopologicalSortVisit(neighbour);
        }
    }
    orderedNodes.push_front(node);
}

void ShaderCompilationSubView::TopologicalSort(const std::set<Node *> &list) {
    visited.clear();
    orderedNodes.clear();
    for (auto &node: list) {
        if (visited.find(node) == visited.end()) {
            TopologicalSortVisit(node);
        }
    }
}

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
        // filter nodes
        FilterNodes(context);

        // find order of nodes
        TopologicalSort(nodes);

        // generate code
        const std::string prefix = CodeGeneration::GetPrefix();
        // 1. function definitions
        const std::string functions = CodeGeneration::GetFunctions(context.finalNode);
        // 2. main function
        const std::string main = CodeGeneration::GetMain(orderedNodes);
        std::cout << prefix << functions << main << std::endl;

        // compile (let preview handle this)
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
