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
#include "Utils/Colors.hpp"
#include "Main/ConfigKeqing.hpp"

void ColorKeqing::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ColorKeqing.png");
    gWorld->setOnQuit([]() {
        Keqing *kq = Keqing::getInstance();
        if (kq == nullptr) return;
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
        kq->setFacingEast(true);
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
        kq->colorAllSprites(colorPicker->getCurrentRGBASlided());
        kq->kqLock(false);
    }, (void *) colorPicker);
    setKqColorButton->addText("Set Color", &Colors::dColorWhite, 20);
    setKqColorButton->changeColor(&Colors::dColorBlue);
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
    resetKqColorButton->addText("Reset Color", &Colors::dColorWhite, 20);
    resetKqColorButton->changeColor(&Colors::dColorGray);
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

        if (gInfo.gPaused) {
            Events::callMainFunc(&gInfo.gRunning, &ConfigKeqing::Run);
            break;
        }

        gWorld->onGameFrame();
        gWindow->clear();
        gWorld->renderSelf();
        gWindow->display();
    }
}
