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
#include "World/Background.hpp"
#include "Entity/Timer.hpp"
#include "Utils/Sound.hpp"
#include "Utils/SQLite3.hpp"
#include "UI/Button.hpp"
#include "Main/HomeMenu.hpp"

int GoalGame::iLevel = 0;
std::vector<LvlFuncPointer> GoalGame::lvlFuncs = {&Level0, &Level1, &Level2};

void GoalGame::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    Block *goalBlock;
    World *gWorld = lvlFuncs[iLevel](&goalBlock);
    goalBlock->setHitBoxAuto();
    goalBlock->resizeToRenderSize();

    Keqing *kq = Keqing::initKeqingForPlay(0, 0);

    auto *mob = new Slime("Blue");
    mob->setDoAI(false);
    mob->setHitBox({1, 4, 14, 12});
    mob->setRenderWHMultiplier(4., 4.);
    mob->moveTo(200, 0);
    mob->setFacingEast(false);
    gWorld->addMonster(mob);

    int bestTime = -1;
    execSQL(("SELECT bestTime "
             "FROM GoalGame "
             "WHERE levelId=" + std::to_string(iLevel)).c_str(),
            [](void *data, int argc, char **argv, char **azColName) -> int {
                if (argc > 0) {
                    int *bestTime = (int *) data;
                    *bestTime = std::stoi(argv[0]);
                }
                return 0;
            },
            (void *) &bestTime);

    char bestTimeStr[32];
    if (bestTime >= 0) sprintf(bestTimeStr, "Best Time : %.2f", (double) bestTime * 0.001);
    else sprintf(bestTimeStr, "Best Time : None");
    Text *bestTimeText = new Text(bestTimeStr, 20, false);
    bestTimeText->moveTo(6., 2.);
    gWorld->addOtherEntity(bestTimeText);

    SDL_Color timerColor = {COLOR_RED_FULL};
    auto *timer = new Timer(6., 26., &timerColor, 30);
    gWorld->addOtherEntity(timer);

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

        if (!timer->hasStarted() && !kq->isInAir()) {
            timer->start();
        }

        if (goalBlock != nullptr && mob->hitBoxCollision(goalBlock)) {
            timer->stop();

            kq->kqLock(true, true);

            Particle *explosionParticle = Particle::pushParticle(PARTICLE_EXPLOSION, 10,
                                                                 3.6, 3.6);
            explosionParticle->moveToEntityCenter(goalBlock);
            Sound::playAudioChunk("res/sfx/particle/TNTExplosion.ogg");

            gWorld->removeBlock(&goalBlock);

            int elapsedTime = timer->getElapsedTime();

            addWinMenu(gWorld, &gInfo.gRunning, elapsedTime);

            if (bestTime < 0) {
                execSQL(("INSERT OR IGNORE INTO GoalGame "
                         "VALUES (" + std::to_string(iLevel) + ", " + std::to_string(elapsedTime) + ")").c_str(),
                        nullptr, nullptr);
            }
            if (elapsedTime < bestTime) {
                execSQL(("UPDATE GoalGame SET bestTime=" +
                         std::to_string(elapsedTime) + " "
                                                       "WHERE levelId=" + std::to_string(iLevel)).c_str(),
                        nullptr, nullptr);
            }
        }

        gWorld->onGameFrame();
        gWindow->clear();
        gWorld->renderSelf();
        gWindow->display();
    }
}

World *GoalGame::Level0(Block **goalBlock) {
    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2000, 720,
                                     "res/gfx/background/GoalGame_0.png");
    gWorld->getBackground()->setRGBAMod(100);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_GRASS,
                            0, 600, 2000);

    *goalBlock = gWorld->addBlock(BLOCK_TNT,
                                  1800, 400, 64, 64);

    return gWorld;
}

World *GoalGame::Level1(Block **goalBlock) {
    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     2964, 1300,
                                     "res/gfx/background/GoalGame_1.png");
    gWorld->getBackground()->setRGBAMod(100);

    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            0, 900, 600);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            600, 700, 300);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            900, 1100, 400);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            1300, 900, 400);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            1700, 700, 400);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            2100, 520, 200);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            2300, 400, 300);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            2600, 600, 300);
    gWorld->addCoveredBlock(BLOCK_DIRT, BLOCK_SNOW,
                            2900, 800, 64);

    *goalBlock = gWorld->addBlock(BLOCK_TNT,
                                  2900, 736, 64, 64);

    return gWorld;
}

World *GoalGame::Level2(Block **goalBlock) {
    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3008, 1692,
                                     "res/gfx/background/GoalGame_2.png");
    gWorld->getBackground()->setRGBAMod(100);

    gWorld->addBlock(BLOCK_QUARTZ,0, 300, 256);
    gWorld->addBlock(BLOCK_QUARTZ,256, 460, 256);
    gWorld->addBlock(BLOCK_QUARTZ,512, 600, 288);
    gWorld->addBlock(BLOCK_QUARTZ,800, 800, 512);
    gWorld->addBlock(BLOCK_QUARTZ,1312, 1000, 768);
    gWorld->addBlock(BLOCK_QUARTZ,2080, 1100, 128);
    gWorld->addBlock(BLOCK_QUARTZ,2208, 1360, 512);
    gWorld->addBlock(BLOCK_QUARTZ,2720, 1510, 176);
    gWorld->addBlock(BLOCK_QUARTZ,2896, 1600, 112);

    *goalBlock = gWorld->addBlock(BLOCK_TNT,
                                  2920, 1520, 64, 64);

    return gWorld;
}

void GoalGame::addWinMenu(World *gWorld, bool *gRunning, int winTime) {
    gWorld->setDisplayMenu(true);

    gWorld->enableColorFilter(128, 128, 128, 128, 0.6);

    SDL_Color tmpColor;

    char winTimeStr[16];
    sprintf(winTimeStr, "Time : %.2f", (double) winTime * 0.001);
    tmpColor = {COLOR_RED_FULL};
    auto *winTimeText = new Text(winTimeStr, &tmpColor,
                                 30, false);
    winTimeText->moveToScreenCenterHorizontal(200);
    gWorld->addMenuEntity(winTimeText);

    auto *retryButton = new Button(0, 0, 200, 100);
    retryButton->moveToEntityBelow(winTimeText, 20);
    retryButton->setOnClickRelease([](Button *self, int mouseX,
                                      int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &GoalGame::Run);
    });
    retryButton->setOnClickReleaseParams((void *) gRunning);
    tmpColor = {COLOR_WHITE_FULL};
    retryButton->addText("Retry", &tmpColor, 22);
    retryButton->changeColor(COLOR_KQ);
    gWorld->addButton(retryButton);

    auto *homeButton = new Button(0, 0, 200, 100);
    homeButton->moveToEntityBelow(retryButton, 20);
    homeButton->setOnClickRelease([](Button *self, int mouseX,
                                     int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &HomeMenu::Run);
    });
    homeButton->setOnClickReleaseParams((void *) gRunning);
    tmpColor = {COLOR_WHITE_FULL};
    homeButton->addText("Exit", &tmpColor, 22);
    homeButton->changeColor(COLOR_KQ);
    gWorld->addButton(homeButton);
}
