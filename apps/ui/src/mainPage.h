//
// Created by Qiaozhi Lei on 5/1/25.
//

#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include "layout.h"
#include "ui.h"
#include "components.h"

using namespace Layout;
using namespace UI;

namespace Pages::Main {
    struct Button {
        LayoutElement root;
        UI_Text label = {
            .wrap = WRAP_NONE,
            .scale = 1.2
        };

        // states
        const char *name;
        int targetPage;
        int *pageIndex;

        UI_Color color = UI_BLACK;

        Button(const char *name, int targetPage, int *pageIndex): name(name), targetPage(targetPage),
                                                                  pageIndex(pageIndex) {
            this->label.text = name;
            root = LayoutBuilder{}.name("Button").size(GROW, FIT)
                    .maxWidth(400)
                    .alignment(CENTER, CENTER).padding(20).mainAxis(VERTICAL)
                    .onMouseEnterFn([&](LayoutElement *layout) {
                        color = UI_GRAY;
                    })
                    .onMouseLeaveFn([&](LayoutElement *layout) {
                        color = UI_BLACK;
                    })
                    .onMouseClickFn([&](LayoutElement *layout) {
                        *this->pageIndex = this->targetPage;
                    })
                    .drawFn([&](LayoutElement *layout) {
                        const std::string image = layout->hovering ? IMAGES"/button-hover.png" : IMAGES"/button.png";
                        UI_DrawImage(image, layout->x, layout->y, layout->width, layout->height);
                    })
                    .children({
                        LayoutBuilder{}.name("Label").pointer(&label.layout),
                    });

            InitReferencePointers(root);
            label.Link();
        }
    };

    struct MainButtons {
        LayoutElement root;
        int *pageIndex;
        std::array<Button, 4> buttons;

        MainButtons(int *pageIndex)
            : pageIndex(pageIndex), buttons{
                  {
                      // note: double braces here
                      {"CONTINUE", 1, pageIndex},
                      {"NEW GAME", 1, pageIndex},
                      {"SETTINGS", 0, pageIndex},
                      {"CREDITS", 0, pageIndex}
                  }
              } {
            root = LayoutBuilder{}.name("Buttons").size(GROW, FIT)
                    .alignment(CENTER, CENTER).padding(10).mainAxis(VERTICAL)
                    .children({
                        std::move(buttons[0].root),
                        std::move(buttons[1].root),
                        std::move(buttons[2].root),
                        std::move(buttons[3].root),
                    });
        }
    };

    struct ButtonHints {
        LayoutElement root;
        InputHint backHint;

        ButtonHints(): backHint("Quit to Desktop", IMAGES"/circle.png") {
            root = LayoutBuilder{}.name("Input Hints").size(GROW, FIT).padding(0)
                    .children({
                        std::move(backHint.root),
                    });
        }
    };

    struct MainPage {
        UI_Text signInLabel = {
            .text = "Signed in as USER",
            .scale = 1
        };
        UI_Image logo = {
            .path = IMAGES"/logo.png",
        };

        LayoutElement root;

        MainButtons buttons;

        ButtonHints hints;

        int *pageIndex;

        MainPage(int *pageIndex): pageIndex(pageIndex), buttons(pageIndex) {
            root = LayoutBuilder{}.name("Main Page")
                    .padding(50).mainAxis(VERTICAL).alignment(CENTER, CENTER)
                    .drawFn([](LayoutElement *layout) {
                        UI_DrawImage(IMAGES"/bg.png", layout->x, layout->y, layout->width, layout->height);
                    })
                    .children({
                        LayoutBuilder{}.name("Sign In Label").size(GROW, 0).pointer(&signInLabel.layout),
                        LayoutBuilder{}.name("Gap").size(GROW, GROW),
                        LayoutBuilder{}.name("Logo").size(GROW, GROW).maxHeight(600)
                        .padding(0).minHeight(400).pointer(&logo.layout),
                        std::move(buttons.root),
                        LayoutBuilder{}.name("Gap").size(GROW, GROW).maxHeight(200),
                        std::move(hints.root),
                    });

            InitReferencePointers(root);
            signInLabel.Link();
            logo.Link();
        }
    };
}

#endif //MAIN_PAGE_H
