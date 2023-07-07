//
// Created by pyrowildx on 01/06/23.
//

#include "Background.hpp"

Background::Background(int screenW, int screenH, int totalWidth, int totalHeight,
                       SDL_Texture *texture) :
        Entity(0, 0, screenW, screenH, texture) {
    this->totalWidth = totalWidth;
    this->totalHeight = totalHeight;
}

bool Background::shouldTranslate() {
    return false;
}

void Background::translate(Entity *entity) {
    double xMid = entity->getX() + (double) entity->getHitBox().w / 2.0;

    imgFrame.x = (int) (xMid - SCREEN_BASE_WIDTH / 2.0);

    if (imgFrame.x < 0) {
        imgFrame.x = 0;
    } else if (imgFrame.x > totalWidth - SCREEN_BASE_WIDTH) {
        imgFrame.x = totalWidth - SCREEN_BASE_WIDTH;
    }

}


int Background::getTotalPixels() const {
    return (totalWidth * totalHeight);
}
