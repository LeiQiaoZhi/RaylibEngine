#ifndef SCROLLPANELRENDERER_H
#define SCROLLPANELRENDERER_H
#include "ISubviewRenderer.h"

class ScrollPanelRenderer final : public ISubviewRenderer {
public:
    ScrollPanelRenderer(int width, int height);

    ~ScrollPanelRenderer() override = default;

    void Begin() override;

    void End() override;

    void Show(Vector2 position, Color tint) override;

    Vector2 GetContentTopLeft() const override;

    Vector2 GetContentSize() const override;

    void SetContentSize(const float width, const float height) override;

    Vector2 GetSize() override {
        return {static_cast<float>(width_), static_cast<float>(height_)};
    }

private:
    int width_;
    int height_;
    Rectangle panelRec = {};
    Rectangle panelContentRec = {};
    Rectangle panelView = {};
    Vector2 panelScroll = {};
};


#endif //SCROLLPANELRENDERER_H
