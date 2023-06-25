//
// Created by pyrowildx on 01/06/23.
//

#include "Background.hpp"

Background::Background(int w, int h, int totalImgWidth, SDL_Texture *texture) :
        Entity(0, 0, w, h, texture) {
    this->totalImgWidth = totalImgWidth;
}

bool Background::shouldTranslate() {
    return false;
}

void Background::translate(Entity *entity) {
    float xMid = entity->getX() + (float) entity->getCollisionRect().w / 2.0f;

    frame.x = (int) (xMid - SCREEN_BASE_WIDTH / 2.0f);

    if (frame.x < 0) {
        frame.x = 0;
    } else if (frame.x > totalImgWidth - SCREEN_BASE_WIDTH) {
        frame.x = totalImgWidth - SCREEN_BASE_WIDTH;
    }

}
