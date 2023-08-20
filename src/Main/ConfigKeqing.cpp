//
// Created by pyrowildx on 06/08/2023.
//

#include "Main/ConfigKeqing.hpp"
#include "Main/ColorKeqing.hpp"
#include "World/World.hpp"
#include "Utils/Global.hpp"
#include "Main/MainInterface.hpp"

void ConfigKeqing::RunImpl() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ConfigKeqing.png");

    auto *swapKqVAButton = new Button(600, 100, 200, 100);
    swapKqVAButton->setOnClickRelease([](Button *self, int mouseX,
                                         int mouseY, void *fParams) {
        const int nLang = 5;
        std::string allKqLangs[nLang] = {"jp", "en", "cn", "kr", "muted"};
        std::string currKqLang = Global::userData[DATA_KQ_VOICE_LANG];
        for (int i = 0; i < nLang; i++) {
            if (currKqLang == allKqLangs[i]) {
                currKqLang = allKqLangs[(i + 1) % nLang];
                Global::saveUserData(DATA_KQ_VOICE_LANG, currKqLang);
                Keqing::getInstance()->setSoundSheetStartPath();
                self->changeText(currKqLang.c_str());
                break;
            }
        }
    });
    SDL_Color tmpColor = {COLOR_WHITE_FULL};
    swapKqVAButton->addText(Global::userData[DATA_KQ_VOICE_LANG].c_str(), &tmpColor,
                            "res/fonts/JetBrainsMono-Regular.ttf", 16);
    swapKqVAButton->changeColor(COLOR_BLUE);
    gWorld->addButton(swapKqVAButton);

    auto *runColorKqButton = new Button(700, 400, 200, 100);
    runColorKqButton->setOnClickRelease([](Button *self, int mouseX,
                                           int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &ColorKeqing::Run);
    });
    tmpColor = {COLOR_WHITE_FULL};
    runColorKqButton->addText("RUN COLOR KEQING", &tmpColor,
                              "res/fonts/JetBrainsMono-Regular.ttf", 16);
    runColorKqButton->changeColor(COLOR_GREEN);
    gWorld->addButton(runColorKqButton);

    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    runColorKqButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));

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
