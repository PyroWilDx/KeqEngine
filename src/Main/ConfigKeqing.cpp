//
// Created by pyrowildx on 06/08/2023.
//

#include "Main/ConfigKeqing.hpp"
#include "Main/ColorKeqing.hpp"
#include "World/World.hpp"
#include "Utils/Global.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "UI/Button.hpp"
#include "Keqing.hpp"
#include "World/Background.hpp"

void ConfigKeqing::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/ConfigKeqing.png");
    gWorld->getBackground()->fitImgToScreen();

    auto *swapKqVAButton = new Button(100, 100, 200, 100);
    swapKqVAButton->setOnClickRelease([](Button *self, int mouseX,
                                         int mouseY, void *fParams) {
        const int nLang = 5;
        std::string allKqLangs[nLang] = {"Jp", "En", "Cn", "Kr", "None"};
        std::string currKqLang = Global::userData[DATA_KQ_VOICE_LANG];
        for (int i = 0; i < nLang; i++) {
            if (currKqLang == allKqLangs[i]) {
                currKqLang = allKqLangs[(i + 1) % nLang];
                Global::saveUserData(DATA_KQ_VOICE_LANG, currKqLang);
                Keqing::getInstance()->setSoundSheetStartPath();
                self->changeText(("VA : " + currKqLang).c_str());
                break;
            }
        }
    });
    SDL_Color tmpColor = {COLOR_WHITE_FULL};
    swapKqVAButton->addText(("VA : " + Global::userData[DATA_KQ_VOICE_LANG]).c_str(),
                            &tmpColor, 16);
    swapKqVAButton->changeColor(COLOR_BLUE);
    gWorld->addButton(swapKqVAButton);

    auto *runColorKqButton = new Button(100, 210, 200, 100);
    runColorKqButton->setOnClickRelease([](Button *self, int mouseX,
                                           int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &ColorKeqing::Run);
    });
    runColorKqButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    tmpColor = {COLOR_WHITE_FULL};
    runColorKqButton->addText("Color Keqing", &tmpColor, 16);
    runColorKqButton->changeColor(COLOR_GREEN);
    gWorld->addButton(runColorKqButton);

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
