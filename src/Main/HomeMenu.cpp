//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/HomeMenu.hpp"
#include "Main/Game1.hpp"
#include "Main/ColorKeqing.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"

void runHomeMenu() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    Global::currentWorld = new World(1280, 720,
                                     1280, 720,
                                     "res/gfx/background/Keqing.png");

    auto *runGame1Button = new Button(400, 0, 200, 100);
    runGame1Button->setCallBack([](void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        *pGRunning = false;
        runGame1();
    });
    SDL_Color tmpColor = {255, 255, 255, 255};
    runGame1Button->addText("RRUN GAME 1", &tmpColor, "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runGame1Button->changeColor(255, 0, 0);
    Global::currentWorld->addButton(runGame1Button);

    auto *runColorKQButton = new Button(400, 200, 200, 100);
    runColorKQButton->setCallBack([](void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        *pGRunning = false;
        runColorKeqing();
    });
    tmpColor = {255, 255, 255, 255};
    runColorKQButton->addText("RRUN CUSTOM MENU 1", &tmpColor, "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runColorKQButton->changeColor(255, 0, 0);
    Global::currentWorld->addButton(runColorKQButton);

    SDL_Event event;
    bool gRunning = true;
    runGame1Button->setOnClickParams((void *) &gRunning);
    runColorKQButton->setOnClickParams((void *) &gRunning);

    while (gRunning) {

        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &gRunning);
        }

        gWindow->clear();
        Global::currentWorld->renderSelf();
        gWindow->display();

    }
}
