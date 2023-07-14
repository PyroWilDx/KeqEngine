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

    auto *gWorld = new World(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             "res/gfx/background/Keqing.png");
    Global::currentWorld = gWorld;

    auto *runGame1Button = new Button(400, 0, 200, 100);
    runGame1Button->setCallBack([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        *pGRunning = false;
        runGame1();
    });
    SDL_Color tmpColor = {255, 255, 255, 255};
    runGame1Button->addText("RRUN GAME 1", &tmpColor, "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runGame1Button->changeColor(255, 0, 0);
    gWorld->addButton(runGame1Button);

    auto *runColorKQButton = new Button(400, 200, 200, 100);
    runColorKQButton->setCallBack([](Button *self, int mouseX, int mouseY, void *onClickParams) {
        bool *pGRunning = (bool *) onClickParams;
        *pGRunning = false;
        runColorKeqing();
    });
    tmpColor = {255, 255, 255, 255};
    runColorKQButton->addText("RRUN CUSTOM MENU 1", &tmpColor, "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runColorKQButton->changeColor(255, 0, 0);
    gWorld->addButton(runColorKQButton);

    SDL_Event event;
    bool gRunning = true;
    runGame1Button->setOnClickParams((void *) &gRunning);
    runColorKQButton->setOnClickParams((void *) &gRunning);

    while (gRunning) {

        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &gRunning);
        }

        gWindow->clear();

        gWorld->renderSelf();

        gWindow->display();
    }
}
