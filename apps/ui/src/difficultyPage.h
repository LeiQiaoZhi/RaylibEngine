//
// Created by Qiaozhi Lei on 5/1/25.
//

#ifndef DIFFICULTYPAGE_H
#define DIFFICULTYPAGE_H

#include "layout.h"
#include "ui.h"
#include "components.h"

using namespace Layout;
using namespace UI;

struct DifficultyInfo {
    UI_Text label = {
        .text = "Normal Difficulty",
        .wrap = WRAP_NONE,
        .scale = 2.0
    };
    UI_Text description = {
        .text =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
        .wrap = WRAP_WORD,
        .scale = 1.0
    };
    LayoutElement root;

    std::array<std::string, 4> titles = {
        "Easy Difficulty", "Normal Difficulty", "Hard Difficulty", "Extreme Difficulty"
    };

    int *selected;

    DifficultyInfo(int *selected) : selected(selected) {
        root = LayoutBuilder{}.name("Difficulty Info").size(GROW, FIT)
                .padding(20).gap(40)
                .mainAxis(VERTICAL).alignment(CENTER, CENTER)
                .updateFn([&](LayoutElement *layout) {
                    label.text = titles[*this->selected].c_str();
                    label.Link();
                })
                .children({
                    LayoutBuilder{}.name("Label").pointer(&label.layout),
                    LayoutBuilder{}.name("Description").width(GROW)
                    .pointer(&description.layout).maxWidth(800),
                });

        InitReferencePointers(root);
        label.Link();
        description.Link();
    }
};

struct DifficultyCard {
    LayoutElement root;
    UI_Text label = {
        .text = "Normal",
        .wrap = WRAP_NONE,
        .scale = 1.2
    };

    // states
    const char *name;
    int id;
    int *selected;

    DifficultyCard(const char *name, int id, int *selected): name(name), id(id), selected(selected) {
        this->label.text = name;
        root = LayoutBuilder{}.name("Card").size(GROW, GROW)
                .alignment(CENTER, CENTER).padding(20).mainAxis(VERTICAL)
                .maxHeight(1000).minHeight(600).maxWidth(400)
                .onMouseEnterFn([&](LayoutElement *layout) {
                    std::cout << "Hovering " << layout->debugName << std::endl;
                    label.scale = 1.5;
                    label.Link();
                })
                .onMouseLeaveFn([&](LayoutElement *layout) {
                    std::cout << "Leave " << layout->debugName << std::endl;
                    label.scale = 1.2;
                    label.Link();
                })
                .onMouseClickFn([&](LayoutElement *layout) {
                    std::cout << "Click " << layout->debugName << std::endl;
                    *this->selected = this->id;
                })
                .children({
                    LayoutBuilder{}.name("Image").size(GROW, GROW)
                    .drawFn([&](LayoutElement *layout) {
                        const UI_Color color = this->id == *this->selected ? UI_WHITE : UI_GRAY;
                        UI_DrawRectangle(layout->x, layout->y, layout->width, layout->height, color);
                    }),
                    LayoutBuilder{}.name("Label").pointer(&label.layout),
                });

        InitReferencePointers(root);
        label.Link();
    }
};

struct DifficultyCards {
    LayoutElement root;
    int *selected;
    std::array<DifficultyCard, 4> cards;

    DifficultyCards(int *selected)
        : selected(selected), cards{
              {
                  // note: double braces here
                  {"Easy", 0, selected},
                  {"Normal", 1, selected},
                  {"Hard", 2, selected},
                  {"Extreme", 3, selected}
              }
          } {
        root = LayoutBuilder{}.name("Difficulty Cards").size(GROW, FIT)
                .alignment(CENTER, CENTER).padding(40)
                .children({
                    std::move(cards[0].root),
                    std::move(cards[1].root),
                    std::move(cards[2].root),
                    std::move(cards[3].root),
                });

        InitReferencePointers(root);
    }
};

struct ButtonHints {
    LayoutElement root;
    InputHint backHint;
    InputHint acceptHint;

    ButtonHints(): backHint("Back", IMAGES"/circle.png"),
                   acceptHint("Accept", IMAGES"/cross.png") {
        root = LayoutBuilder{}.name("Input Hints").size(GROW, FIT).padding(0)
                .children({
                    std::move(backHint.root),
                    std::move(acceptHint.root),
                });
    }
};

struct DifficultyPage {
    UI_Text titleLabel = {
        .text = "SELECT DIFFICULTY",
        .scale = 2
    };
    int selected = 1;

    LayoutElement root;

    DifficultyInfo info = DifficultyInfo(&selected);

    DifficultyCards cards = DifficultyCards(&selected);

    ButtonHints hints;


    DifficultyPage() {
        root = LayoutBuilder{}.name("Difficulty Page")
                .padding(50).mainAxis(VERTICAL).alignment(CENTER, CENTER)
                .drawFn([](LayoutElement *layout) {
                    UI_DrawRectangle(layout->x, layout->y, layout->width, layout->height, UI_BLACK);
                })
                .children({
                    LayoutBuilder{}.name("Title Label").size(GROW, 0)
                    .pointer(&titleLabel.layout),
                    std::move(cards.root),
                    std::move(info.root),
                    LayoutBuilder{}.name("Gap").size(GROW, GROW),
                    std::move(hints.root),
                });

        InitReferencePointers(root);
        titleLabel.Link();
    }
};
;


#endif //DIFFICULTYPAGE_H
