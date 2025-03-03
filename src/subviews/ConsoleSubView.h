#ifndef CONSOLESUBVIEW_H
#define CONSOLESUBVIEW_H
#include "ISubviewRenderer.h"
#include "raylib.h"
#include "../logger/Logger.h"


class Scene;

class ConsoleSubView{
public:
    ConsoleSubView(int width, int height);

    void Render(const Scene &scene, Vector2 position = {0, 0}) const;

private:
    ISubviewRenderer* renderer_;
};


#endif //CONSOLESUBVIEW_H
