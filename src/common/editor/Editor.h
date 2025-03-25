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
    static float TextSizeF() { return GuiGetStyle(DEFAULT, TEXT_SIZE) * 1.0f; }
    static int SmallGap() { return 5; }
    static int MediumGap() { return 10; }
    static int LargeGap() { return 20; }
    static int LabelWidth() { return TextSize() * 5 + LargeGap(); }

    static void BeginIndent(Rectangle &rect, int indent) {
        rect.x += indent;
        rect.width -= indent;
    }

    static void EndIndent(Rectangle &rect, int indent) {
        rect.x -= indent;
        rect.width += indent;
    }

    static Color ContrastBlackOrWhite(Color color) {
        float a = color.a / 255.0f;
        Color bg = BackgroundColor();
        return ((color.r + color.g + color.b) * a + (bg.r + bg.g + bg.b) * (1 - a)) > 3 * 128
                   ? BLACK
                   : WHITE;
    }

    static Color BeginTextColor(Color textColor) {
        const Color original = TextColor();
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(textColor));
        return original;
    }

    static void EndTextColor(Color textColor) {
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(textColor));
    }

    static float TextWidth(const char *str) {
        return MeasureTextEx(GuiGetFont(), str, TextSize(), 0).x;
    }

    static void DrawStatusInfoBox(Rectangle &rect, std::string &statusText, const bool isWarning) {
        if (!statusText.empty()) {
            const int icon = isWarning ? ICON_WARNING : ICON_INFO;
            const char *text = GuiIconText(icon, statusText.c_str());
            const auto originalColor = BeginTextColor(isWarning ? YELLOW : LIGHTGRAY);
            DrawRectangleRec({rect.x, rect.y, rect.width, TextSize() * 1.0f},
                             Fade(ContrastBlackOrWhite(TextColor()), 0.5f));
            GuiLabel({rect.x, rect.y, rect.width, TextSize() * 1.0f}, text);

            const char *cross = GuiIconText(ICON_CROSS_SMALL, "");
            if (GuiLabelButton(
                {rect.x + rect.width - TextSize() * 1.0f, rect.y, TextSize() * 1.0f, TextSize() * 1.0f},
                cross
            )) {
                statusText.clear();
            }

            rect.y += TextSize() + SmallGap();
            EndTextColor(originalColor);
        }
    }

    static Color BeginButtonColor(Color color) {
        Color original = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL));
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(color));
        return original;
    }

    static void EndButtonColor(Color color) {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(color));
    }
};

#endif //EDITOR_H
