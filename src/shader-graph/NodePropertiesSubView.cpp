#include "NodePropertiesSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"


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

    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, TextFormat("Selected Node: %d", context.selectedNodeUID));
    rect.y += Editor::TextSize() * 1.0f + Editor::SmallGap();

    GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
    rect.y += Editor::SmallGap();

    for (auto &node: nodes) {
        node.OnEditorGUI(rect, context);

        GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
        rect.y += Editor::SmallGap();
    }

    // add node
    const float buttonWidth = Editor::TextWidth("Add Node") + Editor::LargeGap();
    Rectangle dropRect = {rect.x, rect.y, rect.width - buttonWidth - Editor::SmallGap(), Editor::TextSize() * 1.5f};
    nodesDropdown.OnEditorGUI(dropRect);

    if (GuiButton(Rectangle{rect.x + rect.width - buttonWidth, rect.y, buttonWidth, Editor::TextSize() * 1.5f},
                  "Add Node")) {
        const std::string path = INTERNAL_ASSET_DIR "/nodes/" + nodesDropdown.GetSelectedOption();
        nlohmann::json j = JsonUtils::JsonFromFile(path);

        nodes.push_back(Node(j));
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap() + nodesDropdown.GetEditorHeight();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
