//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/DebugGame.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Events.hpp"
#include "Entity/Slime.hpp"
#include "Entity/FPSText.hpp"
#include "World/World.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Utils.hpp"
#include "World/Block.hpp"

void DebugGame::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 720,
                                     "res/gfx/background/DebugGame.png");

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 520, 3000);

    Keqing::initKeqingForPlay(0, 0);

    auto *slime = new Slime("Blue");
    slime->setHitBox({1, 4, 14, 12});
    slime->setRenderWHMultiplier(4., 4.);
    slime->moveToDownLeft(400, 720 - 200);
    gWorld->addMonster(slime);

    auto *slime1 = new Slime("Blue");
    slime1->setHitBox({1, 4, 14, 12});
    slime1->setRenderWHMultiplier(4., 4.);
    slime1->moveToDownLeft(1540, 720 - 200);
    gWorld->addMonster(slime1);

    auto *slime2 = new Slime("Blue");
    slime2->setHitBox({1, 4, 14, 12});
    slime2->setRenderWHMultiplier(4., 4.);
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
