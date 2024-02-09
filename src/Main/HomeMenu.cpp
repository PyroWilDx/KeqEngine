//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/HomeMenu.hpp"
#include "Main/ConfigKeqing.hpp"
#include "Main/DebugGame.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Utils/Events.hpp"
#include "Utils/Sound.hpp"
#include "UI/VolumeSlider.hpp"
#include "World/World.hpp"
#include "Utils/Utils.hpp"
#include "Main/GoalGameMenu.hpp"
#include "World/Background.hpp"
#include "Main/SlashEm.hpp"
#include "Utils/Colors.hpp"

void HomeMenu::RunImpl() {
    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;
    Global::gInfo = &gInfo;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     "res/gfx/background/HomeMenu.png");
    gWorld->getBackground()->fitImgToScreen();

    auto *runDebugGameButton = new Button(10, 10, 200, 100);
    runDebugGameButton->setOnClickRelease([](Button *self, int mouseX,
                                             int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &DebugGame::Run);
    });
    runDebugGameButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    runDebugGameButton->addText("Debug Game", &Colors::dColorWhite, 16);
    runDebugGameButton->changeColor(&Colors::dColorBlue);
    gWorld->addButton(runDebugGameButton);

    auto *runGoalGameButton = new Button(10, 120, 200, 100);
    runGoalGameButton->setOnClickRelease([](Button *self, int mouseX,
                                         int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &GoalGameMenu::Run);
    });
    runGoalGameButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    runGoalGameButton->addText("Goal Game", &Colors::dColorBlack, 16);
    runGoalGameButton->changeColor(&Colors::dColorWhite);
    gWorld->addButton(runGoalGameButton);

    auto *runSlashEmButton = new Button(220, 120, 200, 100);
    runSlashEmButton->setOnClickRelease([](Button *self, int mouseX,
                                            int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &SlashEm::Run);
    });
    runSlashEmButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    runSlashEmButton->addText("Slash'Em", &Colors::dColorBlack, 16);
    runSlashEmButton->changeColor(&Colors::dColorWhite);
    gWorld->addButton(runSlashEmButton);

    auto *runConfigKqButton = new Button(10, 230, 200, 100);
    runConfigKqButton->setOnClickRelease([](Button *self, int mouseX,
                                            int mouseY, void *fParams) {
        bool *pGRunning = (bool *) fParams;
        Events::callMainFunc(pGRunning, &ConfigKeqing::Run);
    });
    runConfigKqButton->setOnClickReleaseParams((void *) &(gInfo.gRunning));
    runConfigKqButton->addText("Config Keqing", &Colors::dColorWhite, 16);
    runConfigKqButton->changeColor(&Colors::dColorRed);
    gWorld->addButton(runConfigKqButton);

    int currVolume = std::stoi(Global::userData[DATA_GAME_VOLUME]);
    auto *volumeSlider = new VolumeSlider(880, 10, 390,
                                          &Colors::dColorGreen, currVolume);
    auto fSetVolume = [](Button *self, int mouseX,
                         int mouseY, void *fParams) {
        auto *selfVolumeSlider = (VolumeSlider *) self;
        int currVolumePercent = selfVolumeSlider->getCurrentVolume();
        std::string volumeStr = std::to_string(currVolumePercent);
        Global::saveUserData(DATA_GAME_VOLUME, volumeStr);
        int currVolume = (int) (MIX_MAX_VOLUME * (currVolumePercent / 100.));
        Mix_Volume(-1, currVolume);
    };
    volumeSlider->setOnClick(fSetVolume);
    volumeSlider->setOnClickedMove(fSetVolume);
    gWorld->addButton(volumeSlider);

//    Sound::playAudioMusic("res/sfx/music/NimbleAsLightning.ogg");

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
