//
// Created by pyrowildx on 10/07/2023.
//

#include "Main/ColorKeqing.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Entity/Keqing.hpp"
#include "UI/ColorPicker.hpp"

void ColorKeqing::RunImpl() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ColorKeqing.png");
    gWorld->setOnQuit([]() {
        Keqing *kq = Keqing::getInstance();
        if (kq->isLocked()) {
            Uint32 rgba = cvStringToUint32(Global::userData[DATA_KQ_COLOR]);
            kq->colorCurrSprite(rgba);
            Keqing::getInstance()->unlock();
        }
    });

    Uint32 currRGBA = cvStringToUint32(Global::userData[DATA_KQ_COLOR]);
    auto *colorPicker = new ColorPicker(0, 0, 400, 400, currRGBA);
    colorPicker->setOnClick([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        auto *selfColorPicker = (ColorPicker *) self;
        Uint32 rgba = selfColorPicker->getCurrentRGBA();
        Keqing *kq = Keqing::getInstance();
        kq->lock();
        kq->moveToDownLeft(0, SCREEN_BASE_HEIGHT);
        kq->colorCurrSprite(rgba);
    });
    colorPicker->setOnClickedMove([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        auto *selfColorPicker = (ColorPicker *) self;
        Uint32 rgba = selfColorPicker->getCurrentRGBA();
        Keqing *kq = Keqing::getInstance();
        kq->colorCurrSprite(rgba);
    });
    gWorld->addButton(colorPicker);

    auto *setKqColorButton = new Button(600, 100, 200, 200);
    setKqColorButton->setOnClickRelease([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        auto *colorPicker = (ColorPicker *) onClickParams;
        Keqing *kq = Keqing::getInstance();
        kq->colorAllSprites(colorPicker->getCurrentRGBA());
        kq->unlock();
    }, (void *) colorPicker);
    SDL_Color tmpColor = {COLOR_WHITE_FULL};
    setKqColorButton->addText("SET COLOR", &tmpColor,
                              "res/fonts/JetBrainsMono-Regular.ttf", 20);
    setKqColorButton->changeColor(COLOR_BLUE);
    gWorld->addButton(setKqColorButton);

    auto *resetKqColorButton = new Button(900, 100, 300, 200);
    resetKqColorButton->setOnClickRelease([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        auto *colorPicker = (ColorPicker *) onClickParams;
        Keqing *kq = Keqing::getInstance();
        kq->colorAllSprites(KQ_BASE_COLOR);
        kq->unlock();
        colorPicker->fillPixels(KQ_BASE_COLOR);
    }, (void *) colorPicker);
    tmpColor = {COLOR_WHITE_FULL};
    resetKqColorButton->addText("RESET TO BASE COLOR", &tmpColor,
                                "res/fonts/JetBrainsMono-Regular.ttf", 20);
    resetKqColorButton->changeColor(COLOR_BLUE);
    gWorld->addButton(resetKqColorButton);

    Keqing *kq = Keqing::getInstance();
    kq->moveToDownLeft(0, SCREEN_BASE_HEIGHT);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    gWorld->setRenderKeqing(true);

    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

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
