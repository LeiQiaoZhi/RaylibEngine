#include "ShaderCompilationSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/editor/Editor.h"


void ShaderCompilationSubView::Render(Vector2 position, Context &context) {
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

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Compile")) {
        // find order of nodes

        // generate code

        // compile
    }

    renderer_->End();

    float height = rect.y - topLeft.y + Editor::MediumGap();
    float width = renderer_->GetContentSize().x;
    renderer_->SetContentSize(width, height);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, Fade(WHITE, 0.0));
}
