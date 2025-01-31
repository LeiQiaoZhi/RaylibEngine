#include "ScrollPanelRenderer.h"

#include <raygui.h>

ScrollPanelRenderer::ScrollPanelRenderer(const int width, const int height) : width_(width), height_(height) {
    const int scrollbarWidth = GuiGetStyle(LISTVIEW, SCROLLBAR_WIDTH);
    panelRec = {0, 200, static_cast<float>(width), static_cast<float>(height)};
    panelContentRec = {
        0, 0,
        static_cast<float>(width - scrollbarWidth), static_cast<float>(height)
    };
}

void ScrollPanelRenderer::Begin() {
    GuiScrollPanel(panelRec, nullptr, panelContentRec, &panelScroll, &panelView);
    BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
}

void ScrollPanelRenderer::End() {
    EndScissorMode();
}

void ScrollPanelRenderer::Show(const Vector2 position, Color tint) {
    panelRec.x = position.x;
    panelRec.y = position.y;
}

Vector2 ScrollPanelRenderer::GetContentTopLeft() const {
    return Vector2{panelView.x + panelScroll.x, panelView.y + panelScroll.y};
}

Vector2 ScrollPanelRenderer::GetContentSize() const {
    return Vector2{panelContentRec.width, panelContentRec.height};
}

void ScrollPanelRenderer::SetContentSize(const float width, const float height) {
    panelContentRec.width = width;
    panelContentRec.height = height;
}
