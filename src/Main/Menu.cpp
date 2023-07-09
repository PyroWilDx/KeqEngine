//
// Created by pyrow on 09/07/2023.
//

#include "Main/Menu.hpp"
#include "Main/Game.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"

void runBaseMenu() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    Global::currentWorld = new World(1280, 720,
                                     1280, 720,
                                     "res/gfx/background/Keqing.png");

    auto *button = new Button(400, 0, 200, 100);
    SDL_Color color = {255, 255, 255, 255};
    button->addText("yo", &color, "res/fonts/JetBrainsMono-Regular.ttf", 16);
    button->changeColor(255, 0, 0);
    Global::currentWorld->addButton(button);

    SDL_Event event;
    bool gameRunning = true;

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            Global::windowWidth = event.window.data1;
                            Global::windowHeight = event.window.data2;
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    runGame1();
                    gameRunning = false;
                    break;

                default:
                    break;
            }
        }

        gWindow->clear();
        Global::currentWorld->renderSelf();
        gWindow->display();

    }
}
