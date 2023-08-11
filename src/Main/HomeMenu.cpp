//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/HomeMenu.hpp"
#include "Main/ConfigKeqing.hpp"
#include "Main/Game1.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Utils/Sound.hpp"

void HomeMenu::RunImpl() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/HomeMenu.png");

    auto *runGame1Button = new Button(400, 0, 200, 100);
    runGame1Button->setOnClickRelease([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        Events::callMainFunc(pGRunning, &Game1::Run);
    });
    SDL_Color tmpColor = {COLOR_WHITE_FULL};
    runGame1Button->addText("RUN GAME 1", &tmpColor,
                            "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runGame1Button->changeColor(COLOR_RED);
    gWorld->addButton(runGame1Button);

    auto *runConfigKqButton = new Button(400, 200, 200, 100);
    runConfigKqButton->setOnClickRelease([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        Events::callMainFunc(pGRunning, &ConfigKeqing::Run);
    });
    tmpColor = {COLOR_WHITE_FULL};
    runConfigKqButton->addText("RUN CONFIG KEQING", &tmpColor,
                               "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runConfigKqButton->changeColor(COLOR_RED);
    gWorld->addButton(runConfigKqButton);

//    playAudioMusic("res/sfx/music/NimbleAsLightning.ogg");

    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    runGame1Button->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    runConfigKqButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));

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
