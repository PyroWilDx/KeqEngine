//
// Created by pyrowildx on 01/06/23.
//

#include <cmath>
#include "World/Background.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Random.hpp"
#include "Utils/Global.hpp"

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

    this->baseShakeX = 0;
    this->baseShakeY = 0;
    this->lastShakeX = 0;
    this->lastShakeY = 0;
    this->shakeDuration = 0;
    this->shakeDurationLeft = 0;

    WorldEntity::resizeToRenderSize();
}

void Background::fitImgToScreen() {
    this->imgFrame.w = baseFrameW;
    this->imgFrame.h = baseFrameH;
}

bool Background::shouldTranslate() {
    return false;
}

void Background::fitScreen() {
    imgFrame.x = std::max(imgFrame.x, 0);
    imgFrame.x = std::min(imgFrame.x, totalWidth - SCREEN_BASE_WIDTH);

    imgFrame.y = std::max(imgFrame.y, 0);
    imgFrame.y = std::min(imgFrame.y, totalHeight - SCREEN_BASE_HEIGHT);
}

void Background::translateTo(int x, int y) {
    imgFrame.x = x - lastShakeX;
    imgFrame.y = y - lastShakeY;

    fitScreen();
}

void Background::hardTranslate(Entity *entity) {
    double xMid, yMid;
    entity->getSelfCenter(&xMid, &yMid);

    translateTo((int) (xMid - SCREEN_BASE_WIDTH / 2.),
                (int) (yMid - SCREEN_BASE_HEIGHT / 2.));

    doShake();
}

void Background::lerpTranslate(Entity *entity) {
    double xMid, yMid;
    entity->getSelfCenter(&xMid, &yMid);

    preciseFrameX = lerp(preciseFrameX, xMid, LERP_T);
    preciseFrameY = lerp(preciseFrameY, yMid, LERP_T);

    translateTo((int) std::round(preciseFrameX - SCREEN_BASE_WIDTH / 2. + 30.),
                (int) std::round(preciseFrameY - SCREEN_BASE_HEIGHT / 2.));

    doShake();
}

void Background::shakeScreen(int shakeForceX, int shakeForceY, int sDuration) {
    baseShakeX = shakeForceX;
    baseShakeY = shakeForceY;
    shakeDuration = sDuration;
    shakeDurationLeft = sDuration;
}

void Background::doShake() {
    if (shakeDurationLeft > 0) {
        double dCoeff = ((double) shakeDurationLeft) / shakeDuration;
        int currShakeX = (int) (baseShakeX * dCoeff);
        int currShakeY = (int) (baseShakeY * dCoeff);
        lastShakeX = Random::getRandomInt(-currShakeX, currShakeX);
        lastShakeY = Random::getRandomInt(-currShakeY, currShakeY);
        imgFrame.x += lastShakeX;
        imgFrame.y += lastShakeY;
        shakeDurationLeft -= Global::dt;
    } else {
        lastShakeX = 0;
        lastShakeY = 0;
    }
}

void Background::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect dst = getRenderRect();
    SDL_RenderCopy(gRenderer, imgTexture,
                   &imgFrame, &dst);
}
