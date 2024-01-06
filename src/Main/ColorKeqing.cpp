//
// Created by pyrowildx on 10/07/2023.
//

#include "Main/ColorKeqing.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Keqing.hpp"
#include "UI/ColorPicker.hpp"
#include "World/World.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"

void ColorKeqing::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ColorKeqing.png");
    gWorld->setOnQuit([]() {
        Keqing *kq = Keqing::getInstance();
        if (kq->getIsLocked()) {
            Uint32 rgba = cvStringToUint32(Global::userData[DATA_KQ_COLOR]);
            kq->colorCurrSprite(rgba);
            Keqing::getInstance()->kqLock(false);
        }
    });

    Uint32 currRGBA = cvStringToUint32(Global::userData[DATA_KQ_COLOR]);
    auto *colorPicker = new ColorPicker(40, 40, 400, 400, currRGBA);
    colorPicker->setOnClick([](Button *self, int mouseX,
                               int mouseY, void *fParams) {
        auto *selfColorPicker = (ColorPicker *) self;
        Uint32 rgba = selfColorPicker->getCurrentRGBA();
        Keqing *kq = Keqing::getInstance();
        kq->kqLock(true);
        kq->moveToDownLeft(0, SCREEN_BASE_HEIGHT);
        kq->colorCurrSprite(rgba);
    });
    colorPicker->setOnClickedMove([](Button *self, int mouseX,
                                     int mouseY, void *fParams) {
        auto *selfColorPicker = (ColorPicker *) self;
        Uint32 rgba = selfColorPicker->getCurrentRGBA();
        Keqing *kq = Keqing::getInstance();
        kq->colorCurrSprite(rgba);
    });
    gWorld->addButton(colorPicker);

    auto *setKqColorButton = new Button(600, 100, 200, 200);
    setKqColorButton->setOnClickRelease([](Button *self, int mouseX,
                                           int mouseY, void *fParams) {
        auto *colorPicker = (ColorPicker *) fParams;
        Keqing *kq = Keqing::getInstance();
        kq->colorAllSprites(colorPicker->getCurrentRGBA());
        kq->kqLock(false);
    }, (void *) colorPicker);
    SDL_Color tmpColor = {COLOR_WHITE_FULL};
    setKqColorButton->addText("Set Color", &tmpColor, 20);
    setKqColorButton->changeColor(COLOR_BLUE);
    gWorld->addButton(setKqColorButton);

    auto *resetKqColorButton = new Button(900, 100, 300, 200);
    resetKqColorButton->setOnClickRelease([](Button *self, int mouseX,
                                             int mouseY, void *fParams) {
        auto *colorPicker = (ColorPicker *) fParams;
        Keqing *kq = Keqing::getInstance();
        kq->colorAllSprites(KQ_BASE_COLOR);
        kq->kqLock(false);
        colorPicker->fillPixels(KQ_BASE_COLOR);
    }, (void *) colorPicker);
    tmpColor = {COLOR_WHITE_FULL};
    resetKqColorButton->addText("Reset Color", &tmpColor, 20);
    resetKqColorButton->changeColor(COLOR_GRAY);
    gWorld->addButton(resetKqColorButton);

    Keqing *kq = Keqing::getInstance();
    kq->moveToDownLeft(0, SCREEN_BASE_HEIGHT);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    gWorld->setRenderKeqing(true);

    while (gInfo.gRunning) {
        handleTime();

        while (SDL_PollEvent(&event)) {
            Events::handleBasicEvents(&event, nullptr, &gInfo);
        }
        if (!gInfo.gRunning) break;

        gWorld->onGameFrame();

        gWindow->clear();

        gWorld->renderSelf();

        gWindow->display();
    }
}
