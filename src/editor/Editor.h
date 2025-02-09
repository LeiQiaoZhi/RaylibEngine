#ifndef EDITOR_H
#define EDITOR_H
#include "raylib.h"
#include "raygui.h"

class Editor {
public:
    static Color TextColor() { return GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)); }
    static Color BackgroundColor() { return GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)); }
    static Color PressedColor() { return GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_PRESSED)); }
    static Color FocusedColor() { return GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)); }
    static Color DisabledColor() { return GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_DISABLED)); }
    static int TextSize() { return GuiGetStyle(DEFAULT, TEXT_SIZE); }
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

    static Color BlackOrWhiteBasedOnBackground(Color background) {
        return (background.r + background.g + background.b) / 3 > 128 ? BLACK : WHITE;
    }

    static Color BeginTextColor(Color textColor) {
        const Color original = TextColor();
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(textColor));
        return original;
    }

    static void EndTextColor(Color textColor) {
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(textColor));
    }
};

#endif //EDITOR_H
