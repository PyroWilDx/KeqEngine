//
// Created by pyrowildx on 10/07/2023.
//

#include "Main/ColorKeqing.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Keqing.hpp"
#include "UI/ColorPicker.hpp"

void runColorKeqing() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             "res/gfx/background/ColorKeqing.png");

    auto *colorPicker = new ColorPicker(600, 200, 300, 400);
    colorPicker->setCallBack([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        auto *selfColorPicker = (ColorPicker *) self;
        Uint32 rgba = selfColorPicker->getCurrentRGBA();
        Keqing::getInstance()->colorTexture(rgba >> 24, rgba >> 16, rgba >> 8);
    });
    gWorld->addButton(colorPicker);

    Keqing *kq = Keqing::getInstance();
    kq->moveTo(100, 100);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    kq->setHitbox({(int) (0.0 * KQ_WIDTH_MULTIPLIER),
                   (int) (12.0 * KQ_HEIGHT_MULTIPLIER),
                   (int) (60.0 * KQ_WIDTH_MULTIPLIER),
                   (int) (84.0 * KQ_HEIGHT_MULTIPLIER)});
    gWorld->setRenderKeqing(true);

    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    while (gInfo.gRunning) {

        handleTime();

        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, nullptr, &gInfo);
        }
        if (!gInfo.gRunning) break;

        gWorld->onGameFrame();

        gWindow->clear();

        gWorld->renderSelf();

        gWindow->display();
    }
}
