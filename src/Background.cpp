//
// Created by pyrowildx on 01/06/23.
//

#include "Background.hpp"

Background::Background(int w, int h, int totalImgWidth, SDL_Texture *texture) :
        Entity(0, 0, 0, w, h, false, texture) {
    this->totalImgWidth = totalImgWidth;
}

void Background::move(int dt, int xDirection) {
    int tmp = (int) ((float) dt * BACKGROUND_SPEED);
    frame.x += xDirection * tmp;

    if (frame.x < 0) {
        frame.x = 0;
    } else if (frame.x > totalImgWidth - SCREEN_WIDTH) {
        frame.x = totalImgWidth - SCREEN_WIDTH;
    }
}

void Background::destroy() {
    Entity::destroy();
}
