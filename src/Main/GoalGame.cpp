//
// Created by pyrow on 05/01/2024.
//

#include "Main/GoalGame.hpp"
#include "WindowRenderer.hpp"
#include "World/World.hpp"
#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"
#include "World/Block.hpp"
#include "Keqing.hpp"
#include "Entity/Particle.hpp"
#include "Entity/Slime.hpp"
#include "Entity/FPSText.hpp"

int GoalGame::iLevel = 0;
std::vector<LvlFuncPointer> GoalGame::lvlFuncs = {&Level1, &Level2};

void GoalGame::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 2000,
                                     "res/gfx/background/DebugGame.png");

    lvlFuncs[iLevel]();
    gWorld->addBlock(BLOCK_DIRT,
                     300, 720 - 200, 900, 500);
    gWorld->addBlock(BLOCK_DIRT,
                     1000, 720 - 20, 176, 20);
    gWorld->addBlock(BLOCK_DIRT,
                     1120, 720 - 120, 176, 40);
    gWorld->addBlock(BLOCK_DIRT,
                     1200, 720 - 180, 176, 40);
    gWorld->addBlock(BLOCK_DIRT,
                     1400, 720 - 180, 176, 32);
    gWorld->addBlock(BLOCK_DIRT,
                     0, 720 - 200, 3000, 500);

    Keqing::initKeqingForPlay(0, 0);

    auto *slime = new Slime("Blue");
    slime->setHitBox({1, 4, 14, 12});
    slime->setRenderWHMultiplier(4., 4.);
    slime->moveToDownLeft(400, 720 - 200);
    gWorld->addMonster(slime);

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

        // World
        gWorld->onGameFrame();

        // Window Rendering
        gWindow->clear();

        gWorld->renderSelf();

        gWindow->display();
    }
}

void GoalGame::Level1() {

}

void GoalGame::Level2() {

}
