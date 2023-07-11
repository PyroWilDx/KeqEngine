//
// Created by pyrow on 10/07/2023.
//

#include "Main/ColorKeqing.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"

void runColorKeqing() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    Global::currentWorld = new World(1280, 720,
                                     1280, 720,
                                     "res/gfx/background/Keqing.png");

    SDL_Event event;
    bool gRunning = true;

    while (gRunning) {

        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &gRunning);
        }

        gWindow->clear();
        Global::currentWorld->renderSelf();
        gWindow->display();

    }
}
