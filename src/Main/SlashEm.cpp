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
#include "Text/FPSText.hpp"
#include "Utils/Draw.hpp"
#include "Inventory/Inventory.hpp"

void SlashEm::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2000, 720,
                                     "res/gfx/background/ExploSlime0.png");
    gWorld->getBackground()->setRGBAMod(100);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 520, 2000);

    auto *mobDropper = new MobDropper();
    mobDropper->moveTo(8, 8);
    gWorld->addMonster(mobDropper);

    Keqing *kq = Keqing::initKeqingForPlay(32, 120);

//    auto *gFPSText = new FPSText();
//    gWorld->addOtherEntity(gFPSText);

    while (gInfo.gRunning) {
        handleTime();

        // Events
        int key = KEY_UNDEFINED;
        while (SDL_PollEvent(&event)) {
            Events::handleBasicEvents(&event, &key, &gInfo);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b) {
                if (!Draw::isDisplayingMenu) {
                    gInfo.gPaused = !gInfo.gPaused;
                    if (gInfo.gPaused) kq->getInventory()->showSelf();
                    else kq->getInventory()->hideSelf();
                } else {
                    Draw::removePlayMenu();
                    kq->getInventory()->showSelf();
                }
            }
        }
        if (!gInfo.gRunning) break;

        if (!kq->getInventory()->isShowingSelf()) {
            if (gInfo.gPaused && !Draw::isDisplayingMenu) {
                Draw::drawPlayMenu();
            } else if (!gInfo.gPaused && Draw::isDisplayingMenu) {
                Draw::removePlayMenu();
            }
        } else {
            if (!gInfo.gPaused) kq->getInventory()->hideSelf();
        }

        if (!Draw::isDisplayingMenu && !kq->getInventory()->isShowingSelf()) gWorld->onGameFrame();
        else gWorld->onGameFrameMenu();
        gWindow->clear();
        gWorld->renderSelf();
        gWindow->display();
    }
}
