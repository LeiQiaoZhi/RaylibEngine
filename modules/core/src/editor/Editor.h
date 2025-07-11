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
    static Color ThemeColor() { return {100, 150, 255, 255}; }
    static int TextSize() { return GuiGetStyle(DEFAULT, TEXT_SIZE); }
    static float TextSizeF() { return GuiGetStyle(DEFAULT, TEXT_SIZE) * 1.0f; }
    constexpr static int SmallGap() { return 5; }
    constexpr static int MediumGap() { return 10; }
    constexpr static int LargeGap() { return 20; }
    static int LabelWidth() { return TextSize() * 5 + LargeGap(); }

    static Font GetFont() {
        static Font font;
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
            // font = LoadFont((std::string(INTERNAL_ASSET_DIR) + "/fonts/MapleMono-NF-Regular.ttf").c_str());
            Image image = LoadImage(INTERNAL_ASSET_DIR"/fonts/font512.png");
            if (!IsImageValid(image))
                std::cerr << "Failed to load font image" << std::endl;
            font = LoadFontFromImage(image,WHITE, 322);
        }
        return font;
    }

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
        return MeasureTextEx(GuiGetFont(), str, TextSize(), 1).x;
    }

    static void DrawStatusInfoBox(Rectangle &rect, std::string &statusText, const bool isWarning,
                                  const Color color = LIGHTGRAY, const Color warningColor = YELLOW) {
        if (!statusText.empty()) {
            const int icon = isWarning ? ICON_WARNING : ICON_INFO;
            const char *text = GuiIconText(icon, statusText.c_str());
            const auto originalColor = BeginTextColor(isWarning ? warningColor : color);
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
