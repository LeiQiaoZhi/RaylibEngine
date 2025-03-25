#ifndef RENDERTEXTURERENDERER_H
#define RENDERTEXTURERENDERER_H
#include "raylib.h"
#include "ISubviewRenderer.h"

class RenderTextureRenderer final : public ISubviewRenderer {
public:
    RenderTextureRenderer(int width, int height);

    ~RenderTextureRenderer() override;

    void Begin() override;

    void End() override;

    void Show(Vector2 vector2, Color tint) override;

    Vector2 GetContentTopLeft() const override {
        return {0, 0};
    }

    Vector2 GetContentSize() const override {
        return Vector2{
            static_cast<float>(renderTexture_.texture.width),
            static_cast<float>(renderTexture_.texture.height)
        };
    }

    void SetContentSize(float width, float height) override {
        // TODO
    }

    Vector2 GetSize() override {
        return {static_cast<float>(width_), static_cast<float>(height_)};
    }

private:
    int width_;
    int height_;
    RenderTexture renderTexture_{};
    Rectangle viewRect_{};
};


#endif //RENDERTEXTURERENDERER_H
