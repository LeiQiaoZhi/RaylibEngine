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

    enum Sizing {
        FIXED,
        FIT,
        GROW
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
        Sizing widthSizing = FIXED;
        Sizing heightSizing = FIXED;

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


        Sizing GetMainSizing() const {
            return mainAxis == HORIZONTAL ? widthSizing : heightSizing;
        }

        Sizing GetCrossSizing() const {
            return mainAxis == HORIZONTAL ? heightSizing : widthSizing;
        }

        int GetDimension(const AxisDirection axis) const {
            return axis == HORIZONTAL ? width : height;
        }

        std::string SizingChar(const Sizing sizing) const {
            switch (sizing) {
                case FIT:
                    return "F";
                case GROW:
                    return "G";
            }
            return "";
        }

        Sizing GetSizing(const AxisDirection mainAxis) const {
            return mainAxis == HORIZONTAL ? widthSizing : heightSizing;
        }

        AxisDirection GetCrossAxis() const {
            return mainAxis == HORIZONTAL ? VERTICAL : HORIZONTAL;
        }

        void AddDimension(AxisDirection axis, int add) {
            if (axis == HORIZONTAL) {
                width += add;
            } else {
                height += add;
            }
        }


        // constructor
        UIElement() {
            width = 0;
            height = 0;
        }

        UIElement(const std::string &name, const Sizing wSizing, const Sizing hSizing) {
            debugName = name;
            widthSizing = wSizing;
            heightSizing = hSizing;
        }

        UIElement(const std::string &name, const int w, const int h) {
            debugName = name;
            widthSizing = FIXED;
            heightSizing = FIXED;
            width = w;
            height = h;
        }

        // copy
        UIElement(const UIElement &other) = default;

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
                widthSizing = other.widthSizing;
                heightSizing = other.heightSizing;
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
            widthSizing = other.widthSizing;
            heightSizing = other.heightSizing;
            children = std::move(other.children);
        }
    };

    struct UIBuilder {
        UIElement current;

        UIBuilder &setName(const std::string &name) {
            current.debugName = name;
            return *this;
        }

        UIBuilder &setSize(const int width, const int height) {
            current.width = width;
            current.widthSizing = FIXED;
            current.height = height;
            current.heightSizing = FIXED;
            return *this;
        }

        UIBuilder &setSize(const Sizing widthSizing, const Sizing heightSizing) {
            current.widthSizing = widthSizing;
            current.heightSizing = heightSizing;
            return *this;
        }

        UIBuilder &setSize(const int width, const Sizing heightSizing) {
            current.width = width;
            current.widthSizing = FIXED;
            current.heightSizing = heightSizing;
            return *this;
        }

        UIBuilder &setSize(const Sizing widthSizing, const int height) {
            current.widthSizing = widthSizing;
            current.height = height;
            current.heightSizing = FIXED;
            return *this;
        }

        UIBuilder &setMainAxis(const AxisDirection axis) {
            current.mainAxis = axis;
            return *this;
        }

        UIBuilder &setPadding(const int padding) {
            current.padding = padding;
            return *this;
        }

        UIBuilder &setGap(int gap) {
            current.gap = gap;
            return *this;
        }

        UIBuilder &setChildren(std::vector<UIElement> children) {
            current.children = std::move(children);
            return *this;
        }

        operator UIElement &&() {
            return std::move(current);
        }
    };

    inline void CalculateGrow(UIElement &element) {
        std::cout << "Growing " << element.debugName << std::endl;
        std::vector<UIElement *> childrenGrowMain;
        std::vector<UIElement *> childrenGrowCross;
        int childrenDimensionMain = 0;
        for (auto &child: element.children) {
            if (child.GetSizing(element.mainAxis) == GROW) {
                childrenGrowMain.emplace_back(&child);
            }
            if (child.GetSizing(element.GetCrossAxis()) == GROW) {
                childrenGrowCross.emplace_back(&child);
            }
            childrenDimensionMain += child.GetDimension(element.mainAxis);
        }

        if (childrenGrowMain.size() > 0) {
            int mainRemain = element.GetDimension(element.mainAxis) - childrenDimensionMain
                             - element.padding * 2 - element.gap * (element.children.size() - 1);
            std::cout << "Grow along main, remain: " << mainRemain << std::endl << "\t";
            for (const auto *child: childrenGrowMain)
                std::cout << child->debugName << " ";
            std::cout << std::endl;

            std::vector<UIElement *> childrenToGrow;
            while (mainRemain > 0) {
                // find smallest and second smallest
                int smallest = INT_MAX;
                int smallestChildIndex = -1;
                int secondSmallest = INT_MAX;
                int secondSmallestIndex = -1;
                for (int i = 0; i < childrenGrowMain.size(); i++) {
                    auto *child = childrenGrowMain[i];
                    if (child->GetDimension(element.mainAxis) < smallest) {
                        secondSmallest = smallest;
                        secondSmallestIndex = smallestChildIndex;

                        smallest = child->GetDimension(element.mainAxis);
                        smallestChildIndex = i;
                    }
                    else if (child->GetDimension(element.mainAxis) < secondSmallest) {
                        secondSmallest = child->GetDimension(element.mainAxis);
                        secondSmallestIndex = i;
                    }
                }

                // remove smallest and add it to toGrow
                auto *smallestChild = childrenGrowMain[smallestChildIndex];
                childrenGrowMain.erase(childrenGrowMain.begin() + smallestChildIndex);
                childrenToGrow.emplace_back(smallestChild);

                // grow amount is difference between smallest and second smallest
                int growAmount =  (mainRemain + childrenToGrow.size() - 1) / childrenToGrow.size(); // if there is no second smallest, grow fully
                if (secondSmallestIndex != -1) {
                    growAmount = std::min(growAmount, secondSmallest - smallest);
                }
                std::cout << "Grow pass, remain:" << mainRemain << " grow by:" << growAmount << std::endl;

                // grow
                for (auto *child: childrenToGrow) {
                    child->AddDimension(element.mainAxis, growAmount);
                }

                mainRemain -= growAmount * childrenToGrow.size();
            }
        }

        if (childrenGrowCross.size() > 0) {
            std::cout << "Grow along cross, " << std::endl << "\t";
            for (auto *child: childrenGrowCross) {
                int crossRemain = element.GetDimension(element.GetCrossAxis())
                                  - child->GetDimension(element.GetCrossAxis()) - element.padding * 2;
                std::cout << child->debugName << " " << crossRemain << ",";
                child->AddDimension(element.GetCrossAxis(), crossRemain);
            }
            std::cout << std::endl;
        }
    }

    inline void CalculateSize(UIElement &element) {
        std::cout << "Sizing " << element.debugName << std::endl;

        // recalculate Fit sizing
        int mainSum = element.padding * 2 + element.gap * (element.children.size() - 1);
        int maxCross = 0;
        for (auto &child: element.children) {
            const int main = element.mainAxis == HORIZONTAL ? child.width : child.height;
            int cross = element.mainAxis == HORIZONTAL ? child.height : child.width;

            mainSum += main;
            maxCross = std::max(maxCross, cross);
        }
        maxCross += element.padding * 2;

        if (element.heightSizing != FIXED) {
            element.height = element.mainAxis == HORIZONTAL ? maxCross : mainSum;
        }
        if (element.widthSizing != FIXED) {
            element.width = element.mainAxis == HORIZONTAL ? mainSum : maxCross;
        }
    }

    inline void DFS_Size(UIElement &current) {
        for (auto &child: current.children) {
            DFS_Size(child);
        }
        CalculateSize(current);
    }

    inline void DFS_Grow(UIElement &current) {
        CalculateGrow(current);
        for (auto &child: current.children) {
            DFS_Grow(child);
        }
    }

    inline void CalculateLayout(UIElement &root) {
        //  Sizes
        DFS_Size(root);

        // Grow
        DFS_Grow(root);

        // Positions
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
                std::cout << TextFormat("[%s], %p, %s, %s%ix%s%i, (%i,%i), P%i G%i", child.debugName.c_str(), &child,
                                        child.mainAxis == HORIZONTAL ? "H" : "V",
                                        child.SizingChar(child.widthSizing).c_str(), child.width,
                                        child.SizingChar(child.heightSizing).c_str(), child.height,
                                        child.x, child.y, child.padding, child.gap) << std::endl;
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
                       TextFormat("[%s], %s, %s%ix%s%i, (%i,%i), P%i G%i", current->debugName.c_str(),
                                  current->mainAxis == HORIZONTAL ? "H" : "V",
                                  current->SizingChar(current->widthSizing).c_str(), current->width,
                                  current->SizingChar(current->heightSizing).c_str(), current->height,
                                  current->x, current->y, current->padding, current->gap),
                       {static_cast<float>(current->x), static_cast<float>(current->y)},
                       12, 1, WHITE);

            for (auto &child: current->children) {
                toExplore.emplace_back(&child);
            }
        }
    }
}

#endif //UI_H
