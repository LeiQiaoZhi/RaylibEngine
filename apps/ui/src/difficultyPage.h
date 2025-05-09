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

namespace Pages::Difficulty {
    struct DifficultyInfo {
        UI_Text label = {
            .wrap = WRAP_NONE,
            .scale = 2.0
        };
        UI_Text description = {
            .wrap = WRAP_WORD,
        };
        LayoutElement root;

        std::array<std::string, 4> titles = {
            "Easy Difficulty", "Normal Difficulty", "Hard Difficulty", "Extreme Difficulty"
        };
        std::array<std::string, 4> descriptions = {
            "A gentle introduction for newcomers, with forgiving challenges and plenty of guidance.",
            "A balanced experience that offers satisfying obstacles without being overwhelming.",
            "A rigorous test of skill, designed for players who crave a serious challenge.",
            "An adrenaline-fueled gauntlet where only the most dedicated will prevail."
        };

        int *selected;

        DifficultyInfo(int *selected) : selected(selected) {
            root = LayoutBuilder{}.name("Difficulty Info").size(GROW, FIT)
                    .padding(20).gap(40)
                    .mainAxis(VERTICAL).alignment(CENTER, CENTER)
                    .updateFn([&](LayoutElement *layout) {
                        label.text = titles[*this->selected].c_str();
                        label.Link();
                        description.text = descriptions[*this->selected].c_str();
                        description.Link();
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

    struct TopBar {
        UI_Text titleLabel = {
            .text = "SELECT DIFFICULTY",
            .wrap = WRAP_NONE,
            .scale = 2
        };
        UI_Text backLabel = {
            .text = "Back",
            .wrap = WRAP_NONE,
            .scale = 1.0
        };
        LayoutElement root;

        int *pageIndex;

        TopBar(int *pageIndex) : pageIndex(pageIndex) {
            root = LayoutBuilder{}.name("Top Bar").size(GROW, FIT)
                    .padding(20).gap(40)
                    .children({
                        LayoutBuilder{}.name("Label").pointer(&titleLabel.layout),
                        LayoutBuilder{}.name("Gap").size(GROW, GROW),
                        LayoutBuilder{}.name("Back").size(FIT, GROW).padding(10)
                        .alignment(CENTER, CENTER)
                        .drawFn([](LayoutElement *layout) {
                            UI_DrawRectangle(layout->x, layout->y, layout->width, layout->height, UI_BLACK);
                        })
                        .onMouseClickFn([&](LayoutElement *layout) {
                            std::cout << "Click " << layout->debugName << std::endl;
                            *this->pageIndex = 0;
                        })
                        .children({
                            LayoutBuilder{}.name("Back Label").pointer(&backLabel.layout),
                        })
                    });

            InitReferencePointers(root);
            titleLabel.Link();
            backLabel.Link();
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
        UI_Color color;

        DifficultyCard(const char *name, int id, int *selected): name(name), id(id), selected(selected) {
            this->label.text = name;
            root = LayoutBuilder{}.name("Card").size(GROW, GROW)
                    .alignment(CENTER, CENTER).padding(20).mainAxis(VERTICAL)
                    .maxHeight(1000).minHeight(400).maxWidth(400)
                    .onMouseEnterFn([&](LayoutElement *layout) {
                    })
                    .onMouseLeaveFn([&](LayoutElement *layout) {
                    })
                    .onMouseClickFn([&](LayoutElement *layout) {
                        *this->selected = this->id;
                    })
                    .children({
                        LayoutBuilder{}.name("Image").size(GROW, GROW)
                        .drawFn([&](LayoutElement *layout) {
                            color = UI_BLACK;
                            if (layout->hovering)
                                color = UI_GRAY;
                            if (this->id == *this->selected)
                                color = UI_WHITE;
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
        int *pageIndex;

        LayoutElement root;

        TopBar topBar;

        DifficultyInfo info = DifficultyInfo(&selected);

        DifficultyCards cards = DifficultyCards(&selected);

        ButtonHints hints;


        DifficultyPage(int *pageIndex) : pageIndex(pageIndex), topBar(pageIndex) {
            root = LayoutBuilder{}.name("Difficulty Page")
                    .padding(50).mainAxis(VERTICAL).alignment(CENTER, CENTER)
                    .drawFn([](LayoutElement *layout) {
                        UI_DrawImage(IMAGES"/bg.png", layout->x, layout->y, layout->width, layout->height);
                    })
                    .children({
                        std::move(topBar.root),
                        std::move(cards.root),
                        std::move(info.root),
                        LayoutBuilder{}.name("Gap").size(GROW, GROW),
                        std::move(hints.root),
                    });

            InitReferencePointers(root);
            titleLabel.Link();
        }
    };
}

#endif //DIFFICULTYPAGE_H
