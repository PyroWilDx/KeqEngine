//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/DebugGame.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Events.hpp"
#include "Monster/Slime.hpp"
#include "Text/FPSText.hpp"
#include "World/World.hpp"
#include "Particle.hpp"
#include "Utils/Utils.hpp"
#include "World/Background.hpp"
#include "World/Block.hpp"

void DebugGame::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    gInfo.debugMode = true;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 720,
                                     "res/gfx/background/DebugGame.png");
    gWorld->getBackground()->setRGBAMod(120);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 520, 3000);

    Keqing::initKeqingForPlay(0, 0);

    auto *slime0 = new Slime("Blue");
    slime0->setDoAI(false);
    slime0->moveToDownLeft(400, 720 - 200);
    gWorld->addMonster(slime0);

    auto *slime1 = new Slime("Blue");
    slime1->setDoAI(false);
    slime1->moveToDownLeft(1540, 720 - 200);
    gWorld->addMonster(slime1);

    auto *slime2 = new Slime("Red");
    slime2->setDoAI(false);
    slime2->moveToDownLeft(1600, 720 - 200);
    gWorld->addMonster(slime2);

    auto *gFPSText = new FPSText();
    gWorld->addOtherEntity(gFPSText);

    while (gInfo.gRunning) {
        handleTime();

        // Events
        int key = KEY_UNDEFINED;
        while (SDL_PollEvent(&event)) {
            Events::handleBasicEvents(&event, &key, &gInfo);
        }
        if (!gInfo.gRunning) break;

        // Dev Mode
        if (gInfo.gPaused) {
            if (!gInfo.runFrame) continue;
            else gInfo.runFrame = false;
        }

        gWorld->onGameFrame();
        gWindow->clear();
        gWorld->renderSelf();
        gWorld->renderDebugMode();
        gWindow->display();
    }
}
