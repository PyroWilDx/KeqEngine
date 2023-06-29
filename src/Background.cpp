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
    double xMid = entity->getX() + (double) entity->getHitbox().w / 2.0f;

    frame.x = (int) (xMid - SCREEN_BASE_WIDTH / 2.0f);

    if (frame.x < 0) {
        frame.x = 0;
    } else if (frame.x > totalWidth - SCREEN_BASE_WIDTH) {
        frame.x = totalWidth - SCREEN_BASE_WIDTH;
    }

}


int Background::getTotalPixels() const {
    return (totalWidth * totalHeight);
}
