//
// Created by pyrowildx on 01/06/23.
//

#include "Background.hpp"

Background::Background(int w, int h, int totalImgWidth, SDL_Texture *texture) :
        Entity(0, 0, w, h, texture) {
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

void Background::translate(Entity *entity, int xDiff) {
    int xMid = entity->getX() + entity->getCollisionRect().w / 2;
    int halfX = SCREEN_WIDTH / 2 + frame.x;
    bool isFacingEast = entity->isFacingEast();
    if ((isFacingEast && xMid > halfX) ||
        (!isFacingEast && xMid < halfX)) {
        this->addFrameX(xDiff);
    }
}

void Background::destroy() {
    Entity::destroy();
}
