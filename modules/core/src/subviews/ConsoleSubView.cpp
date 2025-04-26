#include "ConsoleSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"
#include "../logger/Logger.h"
#include "../GCM/Scene.h"
#include "../editor/Editor.h"

ConsoleSubView::ConsoleSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void ConsoleSubView::Render(const Scene &scene, const Vector2 position) const {
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const auto &logger = scene.logger;
    const auto &logEntries = logger.GetLogEntries();

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Console");

    renderer_->Begin();

    Rectangle rect = {
        renderer_->GetContentTopLeft().x + Editor::SmallGap(), renderer_->GetContentTopLeft().y + Editor::SmallGap(),
        renderer_->GetContentSize().x - Editor::SmallGap(), renderer_->GetContentSize().y - Editor::SmallGap() * 2
    };

    for (int i = logEntries.size() - 1; i >= 0; i--) {
        const float timeWidth = Editor::TextWidth(logEntries[i].GetTimestampString().c_str()) + Editor::LargeGap();

        const char* message = GuiIconText(logEntries[i].GetIcon(), logEntries[i].message.c_str());
        GuiLabel({rect.x, rect.y, rect.width - timeWidth, Editor::TextSize() * 1.0f}, message);

        GuiLabel({rect.x + rect.width - timeWidth, rect.y, timeWidth, Editor::TextSize() * 1.0f},
                 logEntries[i].GetTimestampString().c_str());

        rect.y += Editor::TextSize() + Editor::SmallGap();
    }
    renderer_->End();

    renderer_->SetContentSize(renderer_->GetContentSize().x, rect.y);

    const auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, scene.GetTint());
}
