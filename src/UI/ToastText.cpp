//
// Created by pyrow on 01/04/2024.
//

#include "UI/ToastText.hpp"
#include "Utils/Utils.hpp"
#include "Text/Text.hpp"
#include "Utils/Colors.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "WindowRenderer.hpp"

ToastText::ToastText(int renderW, const char *cStr, int showDuration_)
        : WorldEntity(SCREEN_BASE_WIDTH / 2. - renderW / 2., SCREEN_BASE_HEIGHT - 56,
                      renderW, 38, WORLD_TOAST_TEXT) {
    toastText = new Text(WorldEntity::getX() + 8, WorldEntity::getY() + 4,
                         cStr, &Colors::dColorRed, 22);
    toastText->setTranslateBackground(false);
    showDuration = showDuration_;

    Global::gWorld->showToast(this);
}

ToastText::~ToastText() {
    delete toastText;
}

bool ToastText::shouldTranslate() {
    return false;
}

bool ToastText::onGameFrame() {
    bool doNext = WorldEntity::onGameFrame();

    if (doNext) {
        if (getTimeSinceCreation() >= showDuration) Global::gWorld->removeToast();
    }

    return doNext;
}

void ToastText::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect outlineRect = getRenderRect();
    SDL_Rect bgRect = outlineRect;

    outlineRect.x -= 2;
    outlineRect.y -= 2;
    outlineRect.w += 4;
    outlineRect.h += 4;

    WindowRenderer::renderRect(&outlineRect, true,
                               &Colors::dColorLightBlack, gRenderer,
                               true, false);
    WindowRenderer::renderRect(&bgRect, true,
                               &Colors::dColorDarkWhite, gRenderer,
                               true, false);

    toastText->renderSelf(gRenderer);
}
