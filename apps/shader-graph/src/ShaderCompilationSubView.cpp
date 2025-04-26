#include "ShaderCompilationSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "subviews/ScrollPanelRenderer.h"
#include "editor/Editor.h"
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

    // export
    if (GuiTextBox(
        {rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
        exportName.data(), 256, exportNameEditMode)) {
        exportNameEditMode = !exportNameEditMode;
    }
    rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
    GuiComboBox({rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "GLSL;HLSL",
                reinterpret_cast<int *>(&exportType));
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();
    if (GuiButton(
        {rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f},
        "Export")) {
        std::string exportPath = std::string(ASSET_DIR) + "/shaders/" + exportName.c_str()
                                 + (exportType == CodeGeneration::ShaderFileType::GLSL ? ".glsl" : ".hlsl");
        std::string code = GenerateCode(context.FinalNode(), exportType);
        // write code to file
        std::ofstream file(exportPath);
        if (file.is_open()) {
            file << code;
            file.close();
            statusText = "Exported to " + exportPath;
            statusWarning = false;
        } else {
            statusText = "Failed to export to " + exportPath;
            statusWarning = true;
        }
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    GuiLine({rect.x, rect.y, rect.width, 1}, nullptr);
    rect.y += Editor::SmallGap();

    // compile
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
    nodesFoldout.SetLabel(TextFormat("Nodes: %d", nodes.size()));
    nodesFoldout.OnEditorGUI(rect);
    if (!nodesFoldout.IsFolded()) {
        for (auto &node: nodes) {
            GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, node->name.c_str());
            rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
        }
    }
    orderedNodesFoldout.SetLabel(TextFormat("Ordered Nodes: %d", orderedNodes.size()));
    orderedNodesFoldout.OnEditorGUI(rect);
    if (!orderedNodesFoldout.IsFolded()) {
        for (auto &node: orderedNodes) {
            GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, node->name.c_str());
            rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();
        }
    }

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
