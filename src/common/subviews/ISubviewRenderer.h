#ifndef ISUBVIEWRENDERER_H
#define ISUBVIEWRENDERER_H
#include "raylib.h"

#if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24
#endif

class ISubviewRenderer {
public:
    virtual ~ISubviewRenderer() = default;

    virtual void Begin() = 0;

    virtual void End() = 0;

    virtual void Show(Vector2 position, Color tint) = 0;

    virtual Vector2 GetContentTopLeft() const = 0;

    virtual Vector2 GetContentSize() const = 0;

    virtual void SetContentSize(float width, float height) = 0;

    virtual Vector2 GetSize() = 0;
};

#endif //ISUBVIEWRENDERER_H
