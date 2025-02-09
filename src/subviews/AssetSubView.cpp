#include "AssetsSubView.h"

#include <iostream>
#include <raygui.h>
#include <sstream>

#include "ScrollPanelRenderer.h"
#include "../editor/Editor.h"

AssetsSubView::AssetsSubView(const int width, const int height) : rootFileHierarchyProperty(ASSET_DIR, false) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void AssetsSubView::Render(Scene &scene, Vector2 position) {
    if (!scene.root) return;

    // TODO: calc height
    float height = rootFileHierarchyProperty.GetEditorHeight() + Editor::TextSize() * 2.0f + Editor::SmallGap() * 6.0f;
    float width = renderer_->GetContentSize().x;
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

    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                   path, 256, pathEditMode)) {
        pathEditMode = !pathEditMode;
        // TODO: load new path
        if (DirectoryExists(path)) {
            rootFileHierarchyProperty.SetPath(path);
            status = "";
        } else {
            status = GuiIconText(ICON_WARNING, "Invalid path");
        }
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
    if (!status.empty()) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, status.c_str());
        rect.y += Editor::TextSize() + Editor::SmallGap();
    }

    GuiLine({rect.x, rect.y, rect.width, Editor::SmallGap() * 1.0f}, nullptr);
    rect.y += Editor::SmallGap() * 2.0f;
    rect.height -= Editor::SmallGap() * 3.0f + Editor::TextSize() * 1.0f;

    rootFileHierarchyProperty.OnEditorGUI(rect);

    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
