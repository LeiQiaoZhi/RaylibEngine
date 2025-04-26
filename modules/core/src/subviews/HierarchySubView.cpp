#include "HierarchySubView.h"

#include <raygui.h>

#include "ScrollPanelRenderer.h"
#include "../logger/Logger.h"
#include "../editor/Editor.h"
#include "../utils/Utils.h"

HierarchySubView::HierarchySubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void HierarchySubView::Render(Scene &scene, Vector2 position) {
    if (!scene.GetRoot()) {
        return;
    }
    const Color bgColor = Editor::BackgroundColor();
    const int totalCount = scene.GameObjectCount();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, ("Hierarchy, number of game objects: " + std::to_string(totalCount)).c_str());
    Rectangle rect = {
        renderer_->GetContentTopLeft().x + Editor::SmallGap(), renderer_->GetContentTopLeft().y + Editor::SmallGap(),
        renderer_->GetContentSize().x - Editor::SmallGap(), renderer_->GetContentSize().y - Editor::SmallGap() * 2
    };

    renderer_->Begin();

    // Scene save and load
    const float labelWidth = Editor::TextWidth("Scene file") + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "Scene file");
    rect.x += labelWidth;
    const float buttonWidth = Editor::TextWidth("Save") + Editor::LargeGap();
    const float textBoxWidth = rect.width - labelWidth - buttonWidth * 2;
    if (GuiTextBox(Rectangle{rect.x, rect.y, textBoxWidth, Editor::TextSize() * 1.0f},
                   sceneFilename, 256, editMode)) {
        editMode = !editMode;
    }
    rect.x += textBoxWidth;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.0f}, "Load")) {
        LoadScene(scene);
    }
    rect.x += buttonWidth;
    if (GuiButton(Rectangle{rect.x, rect.y, buttonWidth, Editor::TextSize() * 1.0f}, "Save")) {
        const std::string path = std::string(ASSET_DIR) + "/scenes/" + sceneFilename;
        if (!Utils::EndsWith(sceneFilename, ".scene.json")) {
            statusText = "File does not have .scene.json extension";
            statusWarning = true;
        } else if (!FileExists(path.c_str())) {
            Utils::CreateEmptyFile(path);
            statusText = "File does not exist, created, save again";
        } else {
            scene.Save(path.data());
            statusText = "Scene saved";
            statusWarning = false;
        }
    }
    rect.x -= labelWidth + textBoxWidth + buttonWidth;
    rect.y += Editor::TextSize() + Editor::SmallGap();

    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    if (scene.rootFileHierarchyProperty != nullptr)
        scene.rootFileHierarchyProperty->OnEditorGUI(rect);

    renderer_->SetContentSize(renderer_->GetContentSize().x, rect.y - renderer_->GetContentTopLeft().y);
    renderer_->End();

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, scene.GetTint());
}

void HierarchySubView::LoadScene(Scene &scene) {
    const std::string path = std::string(ASSET_DIR) + "/scenes/" + sceneFilename;
    if (!Utils::EndsWith(sceneFilename, ".scene.json")) {
        statusText = "File does not have .scene.json extension";
        statusWarning = true;
    } else if (!FileExists(path.c_str())) {
        statusText = "File does not exist";
        statusWarning = true;
    } else {
        scene.Load(path.data());
        statusText = "Scene loaded";
        statusWarning = false;
    }
}
