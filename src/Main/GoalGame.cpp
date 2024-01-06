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

    World *gWorld = lvlFuncs[iLevel]();

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

World *GoalGame::Level1() {
    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2000, 720,
                                     "res/gfx/background/GoalGame_1.png");

    gWorld->addBlock(BLOCK_DIRT,
                     0, 600, 2000, 160);

    return gWorld;
}

World *GoalGame::Level2() {
    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 2000,
                                     "res/gfx/background/HomeMenu.png");

    // TODO

    return gWorld;
}
