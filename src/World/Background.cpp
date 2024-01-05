//
// Created by pyrowildx on 01/06/23.
//

#include "World/Background.hpp"
#include "Utils/Utils.hpp"

Background::Background(int renderW, int renderH, int totalWidth, int totalHeight,
                       const char *imgPath) :
        WorldEntity(0, 0, renderW, renderH, WORLD_BACKGROUND) {
    this->addImage(imgPath);
    this->totalWidth = totalWidth;
    this->totalHeight = totalHeight;
}

bool Background::shouldTranslate() {
    return false;
}

void Background::translate(Entity *entity) {
    double xMid = entity->getX() + (double) entity->getHitBox().w / 2.;
    double yMid = entity->getY() + (double) entity->getHitBox().y / 2.;

    imgFrame.x = (int) (xMid - SCREEN_BASE_WIDTH / 2.);
    imgFrame.x = std::max(imgFrame.x, 0);
    imgFrame.x = std::min(imgFrame.x, totalWidth - SCREEN_BASE_WIDTH);

    imgFrame.y = (int) (yMid - SCREEN_BASE_HEIGHT / 2.);
    imgFrame.y = std::max(imgFrame.y, 0);
    imgFrame.y = std::min(imgFrame.y, totalHeight - SCREEN_BASE_HEIGHT);
}
