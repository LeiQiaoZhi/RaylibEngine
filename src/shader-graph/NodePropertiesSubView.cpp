#include "NodePropertiesSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"


void NodePropertiesSubView::Render(Vector2 position) {
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
    GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
    rect.y += Editor::SmallGap();

    for (auto &node: nodes) {
        node.OnEditorGUI(rect);

        GuiLine({rect.x, rect.y, rect.x + rect.width, Editor::SmallGap() * 1.0f}, nullptr);
        rect.y += Editor::SmallGap();
    }

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
