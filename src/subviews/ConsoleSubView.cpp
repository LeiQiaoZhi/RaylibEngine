#include "ConsoleSubView.h"
#include <raygui.h>

#include "ScrollPanelRenderer.h"
#include "../logger/Logger.h"

ConsoleSubView::ConsoleSubView(const int width, const int height) {
    renderer_ = new ScrollPanelRenderer(width, height);
}

void ConsoleSubView::Render(const Logger &logger, const Vector2 position) const {
    const int textSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    const auto &logEntries = logger.GetLogEntries();
    renderer_->SetContentSize(renderer_->GetContentSize().x, (logEntries.size() + 1) * textSize + 10);

    renderer_->Begin();
    const Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    for (int i = 0; i < logEntries.size(); i++) {
        const auto topLeft = renderer_->GetContentTopLeft();
        DrawText(logEntries[i].message.c_str(),
                 static_cast<int>(topLeft.x) + 10,
                 (i + 1) * textSize + static_cast<int>(topLeft.y),
                 textSize, textColor);
    }
    renderer_->End();
    renderer_->Show(position, WHITE);
}
