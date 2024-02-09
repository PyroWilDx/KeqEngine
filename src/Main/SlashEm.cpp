//
// Created by pyrow on 08/02/2024.
//

#include "Main/SlashEm.hpp"
#include "Utils/Events.hpp"
#include "Utils/Global.hpp"
#include "WindowRenderer.hpp"
#include "Keqing.hpp"
#include "Utils/Utils.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"
#include "World/Block.hpp"
#include "Monster/MobDropper.hpp"

void SlashEm::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2000, 720,
                                     "res/gfx/background/GoalGame_0.png");
    gWorld->getBackground()->setRGBAMod(100);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 520, 2000);

    auto *mobDropper = new MobDropper();
    mobDropper->moveTo(8, 8);
    gWorld->addMonster(mobDropper);

    Keqing::initKeqingForPlay(32, 120);

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
