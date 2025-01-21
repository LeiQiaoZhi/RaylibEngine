#ifndef GAMESUBVIEW_H
#define GAMESUBVIEW_H
#include "SubView.h"


class GameSubView : public SubView {
public:
    GameSubView(const int width, const int height) : SubView(width, height) {}
    void Begin(const Camera &camera) const;
};


#endif //GAMESUBVIEW_H
