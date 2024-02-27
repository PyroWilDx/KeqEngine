//
// Created by pyrow on 26/02/2024.
//

#include "Main/ShowTest.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Keqing.hpp"
#include "UI/ColorPicker.hpp"
#include "World/World.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Colors.hpp"
#include "Main/ConfigKeqing.hpp"
#include "World/Block.hpp"
#include "Monster/Slime.hpp"
#include "World/Background.hpp"
#include "UI/VolumeSlider.hpp"
#include "Text/Timer.hpp"

void ShowTest::RunImpl() {
    RenderingShow();
//    GameUIShow();
}

void ShowTest::RenderingShow() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    gInfo.debugMode = true;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2000, 720,
                                     "res/gfx/background/GoalGame_0.png");
    gWorld->getBackground()->setRGBAMod(60);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 520, 2000);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 260, 224);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            992, 360, 224);

//    gWorld->addBlock(BLOCK_QUARTZ, 900, 300, 160, 32);
//    gWorld->addBlock(BLOCK_QUARTZ, 1060 - 32, 300 - 32, 160, 32);
//    gWorld->addBlock(BLOCK_QUARTZ, 1360, 200, 160, 32);

    Keqing::initKeqingForPlay(0, 0);

    auto *blueSlime = new Slime("Blue");
    blueSlime->moveToDownLeft(800, 720 - 200);
    blueSlime->setDoAI(false);
    gWorld->addMonster(blueSlime);

    auto *redSlime = new Slime("Red");
    redSlime->moveToDownLeft(1300, 720 - 200);
    redSlime->setFacingEast(false);
    redSlime->setDoAI(false);
    gWorld->addMonster(redSlime);

    auto *redSlime1 = new Slime("Red");
    redSlime1->moveToDownLeft(1600, 720 - 200);
    redSlime1->setFacingEast(false);
    redSlime1->setDoAI(false);
    gWorld->addMonster(redSlime1);

    while (gInfo.gRunning) {
        handleTime();

        // Events
        int key = KEY_UNDEFINED;
        while (SDL_PollEvent(&event)) {
            Events::handleBasicEvents(&event, &key, &gInfo);
        }
        if (!gInfo.gRunning) break;

        gWorld->onGameFrame();
        gWindow->clear();
        gWorld->renderSelf();
        gWindow->display();
    }
}

void ShowTest::GameUIShow() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/DebugGame.png");
    gWorld->getBackground()->setRGBAMod(0);

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

    int currVolume = std::stoi(Global::userData[DATA_GAME_VOLUME]);
    auto *volumeSlider = new VolumeSlider(700, 100, 390,
                                          &Colors::dColorGreen, currVolume);
    gWorld->addButton(volumeSlider);

    auto *timer = new Timer(600, 300, &Colors::dColorWhite,
                            30, &Colors::dColorBlack, 2);
    timer->start();
    gWorld->addOtherEntity(timer);

    auto *clickButton = new Button(600, 460, 300, 160);
    clickButton->setOnClickRelease([](Button *self, int mouseX,
                                      int mouseY, void *fParams) {
        int *clickCount = (int *) fParams;
        *clickCount = *clickCount + 1;
        char newTextCount[32];
        sprintf(newTextCount, "Click Count : %d", *clickCount);
        self->changeText(newTextCount);
    });
    int clickCount = 0;
    clickButton->setOnClickReleaseParams((void *) &clickCount);
    clickButton->addText("Click Count : 0", &Colors::dColorWhite, 16);
    clickButton->changeColor(&Colors::dColorRed);
    gWorld->addButton(clickButton);

    Keqing *kq = Keqing::getInstance();
    kq->moveTo(460, 80);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    kq->kqLock(true);
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
