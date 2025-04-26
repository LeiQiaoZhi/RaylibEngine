//
// Created by Qiaozhi Lei on 4/15/25.
//

#ifndef UI_H
#define UI_H
#include <editor/Editor.h>


int copyCount = 0;
int moveCount = 0;

namespace UI {
    enum AxisDirection {
        HORIZONTAL,
        VERTICAL
    };

    struct UIElement {
        std::string debugName = "Element";
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;
        int padding = 20;
        int gap = 10;
        AxisDirection mainAxis = HORIZONTAL;

        std::vector<UIElement> children;

        int GetMainCoord() const {
            return mainAxis == HORIZONTAL ? x : y;
        }

        int GetCrossCoord() const {
            return mainAxis == HORIZONTAL ? y : x;
        }

        void SetCoord(const int mainCoord, const int crossCoord, const AxisDirection parentAxis) {
            if (parentAxis == HORIZONTAL) {
                x = mainCoord;
                y = crossCoord;
            } else {
                x = crossCoord;
                y = mainCoord;
            }
        }

        int GetDimension(const AxisDirection axis) const {
            return axis == HORIZONTAL ? width : height;
        }

        // constructor
        UIElement() {
            width = 0;
            height = 0;
        }

        UIElement(const std::string &name, int w, int h) {
            debugName = name;
            width = w;
            height = h;
        }

        // copy
        UIElement(const UIElement &other) {
            copyCount++;

            width = other.width;
            height = other.height;
            children = other.children;
        }

        // assign
        UIElement &operator=(const UIElement &other) {
            copyCount++;

            if (this != &other) {
                debugName = other.debugName;
                width = other.width;
                height = other.height;
                padding = other.padding;
                gap = other.gap;
                x = other.x;
                y = other.y;
                mainAxis = other.mainAxis;
                children = other.children;
            }
            return *this;
        }

        // move
        UIElement(UIElement &&other) noexcept {
            moveCount++;

            debugName = std::move(other.debugName);
            width = other.width;
            height = other.height;
            padding = other.padding;
            gap = other.gap;
            x = other.x;
            y = other.y;
            mainAxis = other.mainAxis;
            children = std::move(other.children);
        }
    };


    inline void OpenElement(UIElement &element, UIElement &parent) {
        std::cout << "Opening Element " << element.debugName << std::endl;
    }

    inline void CloseElement(UIElement &element) {
        std::cout << "Closing Element " << element.debugName << std::endl;
    }

    inline void DFS(UIElement &current, UIElement &parent) {
        OpenElement(current, parent);
        for (auto &child: current.children) {
            DFS(child, current);
        }
        CloseElement(current);
    }

    inline void CalculateLayout(UIElement &root) {
        // TODO: calculate sizes
        UIElement rootParent;
        rootParent.x = 0;
        rootParent.y = 0;
        DFS(root, rootParent);

        // calculate positions
        root.x = 0;
        root.y = 0;
        std::vector<UIElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            UIElement *current = toExplore.back();
            toExplore.pop_back();

            // calculate positions of children
            int mainAxisProcessed = 0;
            for (auto &child: current->children) {
                if (current->mainAxis == VERTICAL) {
                }
                const int main = current->GetMainCoord() + current->padding + mainAxisProcessed;
                const int cross = current->GetCrossCoord() + current->padding;
                child.SetCoord(main, cross, current->mainAxis);

                mainAxisProcessed += child.GetDimension(current->mainAxis) + current->gap;
            }

            for (auto &child: current->children) {
                std::cout << child.debugName << ": "
                        << child.x << ", " << child.y << std::endl;
                toExplore.emplace_back(&child);
            }
        }
    }

    inline void DrawUIDebug(UIElement &root) {
        std::vector<UIElement *> toExplore = {&root};
        while (!toExplore.empty()) {
            UIElement *current = toExplore.back();
            toExplore.pop_back();

            DrawRectangleLines(current->x, current->y, current->width, current->height, GRAY);

            DrawTextEx(Editor::GetFont(),
                       TextFormat("%s, %s, %ix%i, (%i,%i)", current->debugName.c_str(),
                                  current->mainAxis == HORIZONTAL ? "H" : "V",
                                  current->width, current->height, current->x, current->y),
                       {static_cast<float>(current->x), static_cast<float>(current->y)},
                       12, 1, WHITE);

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }
}

#endif //UI_H
