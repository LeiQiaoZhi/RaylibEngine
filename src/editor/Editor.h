#ifndef EDITOR_H
#define EDITOR_H
#include "raylib.h"
#include "raygui.h"

class Editor {
public:
    static inline Color TextColor() { return GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)); }
    static inline Color BackgroundColor() { return GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)); }
    static inline Color PressedColor() { return GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_PRESSED)); }
    static inline Color FocusedColor() { return GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)); }
    static inline Color DisabledColor() { return GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED)); }
    static inline int TextSize() { return GuiGetStyle(DEFAULT, TEXT_SIZE); }
    static int SmallGap() { return 5; }
    static int MediumGap() { return 10; }
    static int LargeGap() { return 20; }

    static void BeginIndent(Rectangle &rect, int indent) {
        rect.x += indent;
        rect.width -= indent;
    }

    static void EndIndent(Rectangle &rect, int indent) {
        rect.x -= indent;
        rect.width += indent;
    }
};

#endif //EDITOR_H
