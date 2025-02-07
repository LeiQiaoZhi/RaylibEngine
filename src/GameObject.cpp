//
// Created by Qiaozhi Lei on 2/6/25.
//

#include "GameObject.h"

int GameObject::RecursiveChildCount() const {
    int count = 0;
    for (const auto* child : children) {
        count += child->RecursiveChildCount() + 1;
    }
    return count;
}
