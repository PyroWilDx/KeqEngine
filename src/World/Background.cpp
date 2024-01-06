//
// Created by pyrowildx on 01/06/23.
//

#include <cmath>
#include "World/Background.hpp"
#include "Utils/Utils.hpp"

Background::Background(int renderW, int renderH, int totalWidth, int totalHeight,
                       const char *imgPath) :
        WorldEntity(0, 0, renderW, renderH, WORLD_BACKGROUND) {
    this->addImage(imgPath);
    this->totalWidth = totalWidth;
    this->totalHeight = totalHeight;
    this->preciseFrameX = 0.;
    this->preciseFrameY = 0.;

    this->baseFrameW = imgFrame.w;
    this->baseFrameH = imgFrame.h;

    resizeToRenderSize();
}

void Background::fitImgToScreen() {
    this->imgFrame.w = baseFrameW;
    this->imgFrame.h = baseFrameH;
}

bool Background::shouldTranslate() {
    return false;
}

void Background::translateTo(int x, int y) {
    imgFrame.x = x;
    imgFrame.x = std::max(imgFrame.x, 0);
    imgFrame.x = std::min(imgFrame.x, totalWidth - SCREEN_BASE_WIDTH);

    imgFrame.y = y;
    imgFrame.y = std::max(imgFrame.y, 0);
    imgFrame.y = std::min(imgFrame.y, totalHeight - SCREEN_BASE_HEIGHT);
}

void Background::hardTranslate(Entity *entity) {
    double xMid, yMid;
    entity->getSelfCenter(&xMid, &yMid);

    translateTo((int) (xMid - SCREEN_BASE_WIDTH / 2.),
                (int) (yMid - SCREEN_BASE_HEIGHT / 2.));
}

void Background::lerpTranslate(Entity *entity) {
    double xMid, yMid;
    entity->getSelfCenter(&xMid, &yMid);

    preciseFrameX = lerp(preciseFrameX, xMid, LERP_T);
    preciseFrameY = lerp(preciseFrameY, yMid, LERP_T);

    translateTo((int) std::round(preciseFrameX - SCREEN_BASE_WIDTH / 2. + 30.),
                (int) std::round(preciseFrameY - SCREEN_BASE_HEIGHT / 2.));
}
