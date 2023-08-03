//
// Created by pyrowildx on 11/07/2023.
//

#include "Utils/Events.hpp"
#include "Utils/Global.hpp"
#include "Main/HomeMenu.hpp"

void onWindowResize(int newW, int newH) {
    Global::windowWidth = newW;
    Global::windowHeight = newH;

    double wCoeff = (double) newW / SCREEN_BASE_WIDTH;
    double hCoeff = (double) newH / SCREEN_BASE_HEIGHT;
    int newRenderW, newRenderH;
    if (wCoeff < hCoeff) {
        newRenderW = newW;
        newRenderH = SCREEN_BASE_HEIGHT * wCoeff;
    }
    if (wCoeff == hCoeff) {
        newRenderW = newW;
        newRenderH = newH;
    }
    if (hCoeff < wCoeff) {
        newRenderW = SCREEN_BASE_WIDTH * hCoeff;
        newRenderH = newH;
    }

    int wDiff = newW - newRenderW;
    int hDiff = newH - newRenderH;
    int epsilonDiff = 20;
    if ((wDiff > 1 && wDiff < epsilonDiff) || (hDiff > 1 && hDiff < epsilonDiff)) {
        int resizeW = (wDiff > 1 && wDiff < epsilonDiff) ? newRenderW : newW;
        int resizeH = (hDiff > 1 && hDiff < epsilonDiff) ? newRenderH : newH;
        SDL_SetWindowSize(WindowRenderer::getInstance()->getWindow(),
                          resizeW, resizeH);
        onWindowResize(resizeW, resizeH);
        return;
    }

    if (wDiff == 1) {
        newRenderW++;
        wDiff--;
    }
    if (hDiff == 1) {
        newRenderH++;
        hDiff--;
    }

    Global::renderWindowWidth = newRenderW;
    Global::renderWindowHeight = newRenderH;

    SDL_Rect renderClipper;
    SDL_Rect *pRenderClipper = &renderClipper;
    if ((newW - newRenderW) != 0) {
        renderClipper = {wDiff / 2,
                         hDiff / 2,
                         newRenderW,
                         newRenderH};
    } else {
        pRenderClipper = nullptr;
    }
    SDL_RenderSetClipRect(WindowRenderer::getInstance()->getRenderer(),
                          pRenderClipper);
}

void handleBasicEvents(SDL_Event *event, int *pKey, gStateInfo *gInfo) {
    int SDLKey;
    int key;

    switch (event->type) {
        case SDL_QUIT:
            callMainFunc(&gInfo->gRunning, nullptr);
            break;

        case SDL_WINDOWEVENT:
            switch (event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    onWindowResize(event->window.data1, event->window.data2);
                    break;
            }
            break;

        case SDL_KEYDOWN:
            SDLKey = event->key.keysym.sym;
            switch (SDLKey) {
                case SDLK_ESCAPE:
                    callMainFunc(&gInfo->gRunning, &runHomeMenu);
                    break;

                case SDLK_F11: {
                    SDL_Window *gWindow = WindowRenderer::getInstance()->getWindow();
                    int windowFlag = (Global::isWindowFullscreen) ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(gWindow,
                                            windowFlag);
                    Global::isWindowFullscreen = !Global::isWindowFullscreen;
                    int newW, newH;
                    SDL_GetWindowSize(gWindow, &newW, &newH);
                    onWindowResize(newW, newH);
                    break;
                }

                case SDLK_BACKSPACE:
                    gInfo->gPaused = !gInfo->gPaused;
                    if (!gInfo->gPaused) Global::currentTime = getTime();
                    break;

                case SDLK_RETURN:
                    if (gInfo->gPaused) {
                        gInfo->runFrame = true;
                        Global::dt = 10;
                    }
                    break;

                default:
                    key = updatePressedKeys(SDLKey, true, true);
                    break;
            }
            break;

        case SDL_KEYUP:
            SDLKey = event->key.keysym.sym;
            key = updatePressedKeys(SDLKey, false, true);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            SDLKey = event->button.button;
            key = updatePressedKeys(SDLKey,
                                    (event->type == SDL_MOUSEBUTTONDOWN),
                                    false);
            int mouseX = event->button.x;
            int mouseY = event->button.y;
            getMouseAbsoluteXY(&mouseX, &mouseY);
            Global::currentWorld->clickPixel(mouseX, mouseY, event->type);
            break;
        }

        default:
            break;
    }

    if (pKey != nullptr) *pKey = key;
}

void callMainFunc(bool *gRunningLastMain, void (*gMain)()) {
    *gRunningLastMain = false;
    Particle::removeAllParticles();
    Global::deleteWorld();
    if (gMain != nullptr) gMain();
}
