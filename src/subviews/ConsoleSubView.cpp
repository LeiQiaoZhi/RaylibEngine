#include "ConsoleSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"
#include "../logger/Logger.h"

ConsoleSubView::ConsoleSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
}

void ConsoleSubView::Render(const Logger &logger, const Vector2 position) const {
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const auto &logEntries = logger.GetLogEntries();
    renderer_->SetContentSize(renderer_->GetContentSize().x, (logEntries.size() + 1) * textSize + 10);

    GuiStatusBar({
                     position.x, position.y,
                     renderer_->GetSize().x,RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT
                 }, "Console");
    renderer_->Begin();
    const Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    for (int i = 0; i < logEntries.size(); ++i) {
        const auto topLeft = renderer_->GetContentTopLeft();
        DrawText(logEntries[i].message.c_str(),
                 static_cast<int>(topLeft.x) + 10,
                 (logEntries.size() - i) * textSize + static_cast<int>(topLeft.y),
                 textSize, textColor);
    }
    renderer_->End();

    auto contentPosition = Vector2{position.x, position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
    renderer_->Show(contentPosition, WHITE);
}
