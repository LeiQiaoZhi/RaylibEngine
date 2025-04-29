//
// Created by Qiaozhi Lei on 4/29/25.
//

#include "ui.h"
#include <raylib.h>

void Raylib_DrawText(const char *str, const int x, const int y, float scale) {
    DrawText(str, x, y, 20 * scale, WHITE);
}

void UI_Raylib_Init() {
  UI::drawTextFn = &Raylib_DrawText;
}


