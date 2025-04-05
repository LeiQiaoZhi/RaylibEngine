#include "NodePropertiesSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"
#include "nodes/NodeGroup.h"


void NodePropertiesSubView::Render(Vector2 position, Context &context) {
    const auto topLeft = renderer_->GetContentTopLeft();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Node Properties");

    Rectangle rect = {
        topLeft.x + Editor::SmallGap(), topLeft.y + Editor::SmallGap(),
        renderer_->GetContentSize().x - Editor::SmallGap() * 2,
        renderer_->GetContentSize().y - Editor::SmallGap() * 2
    };
    renderer_->Begin();

    // save and load
    const float labelWidth = Editor::TextWidth("Graph file") + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "Graph file");
    rect.x += labelWidth;
    const float buttonWidth = Editor::TextWidth("Save") + Editor::LargeGap();
    const float textBoxWidth = rect.width - labelWidth - buttonWidth * 2;
    if (GuiTextBox(Rectangle{rect.x, rect.y, textBoxWidth, Editor::TextSize() * 1.0f},
                   graphFilename, 256, editMode)) {
        editMode = !editMode;
    }
    rect.x += textBoxWidth;
    const std::string path = std::string(ASSET_DIR) + "/graphs/" + graphFilename;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.0f}, "Load")) {
        if (!Utils::EndsWith(graphFilename, ".graph.json")) {
            statusText = "File does not have .graph.json extension";
            statusWarning = true;
        } else if (!FileExists(path.c_str())) {
            statusText = "File does not exist";
            statusWarning = true;
        } else {
            context.LoadGraph(path);
            statusText = "Graph loaded";
            statusWarning = false;
        }
    }
    rect.x += buttonWidth;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.0f}, "Save")) {
        if (!Utils::EndsWith(graphFilename, ".graph.json")) {
            statusText = "File does not have .graph.json extension";
            statusWarning = true;
        } else if (!FileExists(path.c_str())) {
            Utils::CreateEmptyFile(path);
            statusText = "File does not exist, created, save again";
        } else {
            context.SaveGraph(path);
            statusText = "Graph saved";
            statusWarning = false;
        }
    }
    rect.x -= labelWidth + textBoxWidth + buttonWidth;
    rect.y += Editor::TextSize() + Editor::SmallGap();

    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);


    // nodes
    nodesFoldout.OnEditorGUI(rect);
    if (!nodesFoldout.IsFolded()) {
        GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
        rect.y += Editor::SmallGap();

        for (auto &node: context.nodes) {
            node.OnEditorGUI(rect, context);

            GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
            rect.y += Editor::SmallGap();
        }
    }
    // groups
    groupsFoldout.OnEditorGUI(rect);
    if (!groupsFoldout.IsFolded()) {
        GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
        rect.y += Editor::SmallGap();

        for (auto &group: context.nodeGroups) {
            group.OnEditorGUI(rect, context);

            GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
            rect.y += Editor::SmallGap();
        }
    }


    // add node
    const float addWidth = Editor::TextWidth("Add Node") + Editor::LargeGap();
    Rectangle dropRect = {rect.x, rect.y, rect.width - addWidth - Editor::SmallGap(), Editor::TextSize() * 1.5f};
    nodesDropdown.OnEditorGUI(dropRect);

    if (GuiButton(Rectangle{rect.x + rect.width - addWidth, rect.y, addWidth, Editor::TextSize() * 1.5f},
                  "Add Node")) {
        const std::string path = INTERNAL_ASSET_DIR "/nodes/" + nodesDropdown.GetSelectedOption() + ".json";
        nlohmann::json j = JsonUtils::JsonFromFile(path);

        context.nodes.emplace_back(j);
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap() + nodesDropdown.GetEditorHeight();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
