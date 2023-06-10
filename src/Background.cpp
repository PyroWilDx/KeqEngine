//
// Created by pyrowildx on 01/06/23.
//

#include "Background.hpp"

Background::Background(int w, int h, int totalImgWidth, SDL_Texture *texture) :
        Entity(0, 0, 0, w, h, false, texture) {
    this->totalImgWidth = totalImgWidth;
}

void Background::addFrameX(int x) {
    frame.x += x;

    if (frame.x < 0) {
        frame.x = 0;
    } else if (frame.x > totalImgWidth - SCREEN_WIDTH) {
        frame.x = totalImgWidth - SCREEN_WIDTH;
    }
}

void Background::destroy() {
    Entity::destroy();
}
