//
// Created by pyrow on 05/01/2024.
//

#include "Main/ExploSlimeMenu.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "World/World.hpp"
#include "Utils/Global.hpp"
#include "UI/Button.hpp"
#include "Main/ExploSlime.hpp"
#include "World/Background.hpp"
#include "Utils/Colors.hpp"
#include "Main/HomeMenu.hpp"

void ExploSlimeMenu::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ExploSlimeMenu.png");
    gWorld->getBackground()->fitImgToScreen();

    const int nLevel = (int) ExploSlime::lvlFuncs.size();
    const int padding = 10;
    int x = padding;
    int y = padding;
    const int renderW = 100;
    const int renderH = 60;
    for (int i = 0; i < nLevel; i++) {
        auto *lButton = new Button(x, y, renderW, renderH);
        x += renderW + padding;
        if (x > SCREEN_BASE_WIDTH) {
            x = padding;
            y += renderH + padding;
        }
        lButton->setOnClickRelease([](Button *self, int mouseX,
                                           int mouseY, void *fParams) {
            auto *args = (f2Params *) fParams;
            int iLevel = *(int *) args->fParam0;
            bool *pGRunning = (bool *) args->fParam1;
            ExploSlime::iLevel = iLevel;
            Events::callMainFunc(pGRunning, &ExploSlime::Run);
        });
        auto *fParams = new f2Params;
        int *fParam0 = new int;
        *fParam0 = i;
        fParams->fParam0 = (void *) fParam0;
        fParams->fParam1 = (void *) &(gInfo.gRunning);
        lButton->setOnClickReleaseParams((void *) fParams);
        lButton->addText(("Lvl " + std::to_string(i)).c_str(),
                         &Colors::dColorWhite, 16);
        lButton->changeColor(&Colors::dColorGreen);
        gWorld->addButton(lButton);
    }

    while (gInfo.gRunning) {
        handleTime();

        while (SDL_PollEvent(&event)) {
            Events::handleBasicEvents(&event, nullptr, &gInfo);
        }
        if (!gInfo.gRunning) break;

        if (gInfo.gPaused) {
            Events::callMainFunc(&gInfo.gRunning, &HomeMenu::Run);
            break;
        }

        gWorld->onGameFrame();
        gWindow->clear();
        gWorld->renderSelf();
        gWindow->display();
    }
}
