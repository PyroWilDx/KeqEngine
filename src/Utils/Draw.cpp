//
// Created by pyrow on 05/02/2024.
//

#include "Utils/Draw.hpp"
#include "UI/Button.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "Main/HomeMenu.hpp"
#include "Utils/Colors.hpp"

void Draw::initDraw() {

}

void Draw::cleanUp() {

}

Button *Draw::drawRetryButton(SDL_Rect *btRect, Entity *upperEntity,
                           void (*fOnClickRelease)(Button *, int, int, void *),
                           void *fParams) {
    auto *retryButton = new Button(btRect->x, btRect->y,
                                   btRect->w, btRect->h);
    if (upperEntity != nullptr) {
        retryButton->moveToEntityBelow(upperEntity, DEFAULT_PADDING);
    }
    retryButton->setOnClickRelease(fOnClickRelease);
    retryButton->setOnClickReleaseParams(fParams);
    retryButton->addText("Retry", &Colors::dColorWhite, 22);
    retryButton->changeColor(&Colors::dColorKq);
    Global::gWorld->addButton(retryButton);

    return retryButton;
}

Button *Draw::drawHomeButton(SDL_Rect *btRect, Entity *upperEntity, bool *gRunning) {
    auto *homeButton = new Button(btRect->x, btRect->y,
                                  btRect->w, btRect->h);
    if (upperEntity != nullptr) {
        homeButton->moveToEntityBelow(upperEntity, DEFAULT_PADDING);
    }
    homeButton->setOnClickRelease([](Button *self, int mouseX,
                                     int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &HomeMenu::Run);
    });
    homeButton->setOnClickReleaseParams((void *) gRunning);
    homeButton->addText("Exit", &Colors::dColorWhite, 22);
    homeButton->changeColor(&Colors::dColorKq);
    Global::gWorld->addButton(homeButton);

    return homeButton;
}
