//
// Created by Qiaozhi Lei on 4/29/25.
//

#ifndef UI_H
#define UI_H

#include "layout.h"

namespace UI {
    using DrawTextFn = void(*)(const char *, int, int, float);
    inline DrawTextFn drawTextFn = nullptr;

    inline void UI_DrawText(const char *str, const int x, const int y, float scale) {
        if (drawTextFn != nullptr) {
            drawTextFn(str, x, y, scale);
        }
    }

    struct UI_Text {
        Layout::LayoutElement *layout;
        const char *text;
        float scale = 1.0;

        void Draw() {
            UI_DrawText(text, layout->x, layout->y, scale);
        }
    };
}

#endif //UI_H
