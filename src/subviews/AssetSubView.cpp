#include "AssetsSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "ScrollPanelRenderer.h"
#include "../editor/Editor.h"

AssetsSubView::AssetsSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void AssetsSubView::Render(Scene &scene, Vector2 position) const {
    if (!scene.root) return;
    const auto assetPathStr = "Assets path: " + std::string(ASSET_DIR);
    FilePathList files = LoadDirectoryFilesEx(ASSET_DIR, NULL, true);

    // TODO: calc height
    float height = renderer_->GetContentSize().y;
    float width = MeasureText(assetPathStr.c_str(), Editor::TextSize());

    renderer_->SetContentSize(width, height);

    const auto topLeft = renderer_->GetContentTopLeft();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Assets");

    Rectangle rect = {
        topLeft.x + Editor::SmallGap(), topLeft.y + Editor::SmallGap(),
        renderer_->GetContentSize().x - Editor::SmallGap() * 2,
        renderer_->GetContentSize().y - Editor::SmallGap() * 2
    };
    renderer_->Begin();
    GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, assetPathStr.c_str());
    rect.y += Editor::TextSize() + Editor::SmallGap();
    for (int i =0; i<files.count; i++) {
        const char* completeFilePath = files.paths[i];
        const char* file = GetFileName(completeFilePath);
        if (file[0] == '.')
            continue;
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, file);
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
