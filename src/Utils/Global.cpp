//
// Created by pyrowildx on 25/06/2023.
//

#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"

int Global::windowWidth = SCREEN_BASE_WIDTH;
int Global::windowHeight = SCREEN_BASE_HEIGHT;
int Global::renderWindowWidth = SCREEN_BASE_WIDTH;
int Global::renderWindowHeight = SCREEN_BASE_HEIGHT;
bool Global::isWindowFullscreen = false;

bool Global::pressedKeys[KEY_ENUM_N];
int Global::pressedTime[KEY_ENUM_N];
int Global::lastPressedTime[KEY_ENUM_N];

int Global::currentTime = 0;
int Global::dt = 0;

World *Global::currentWorld = nullptr;

void Global::initGlobal() {
    for (int i = 0; i < KEY_ENUM_N; i++) {
        pressedKeys[i] = false;
        pressedTime[i] = 0;
        lastPressedTime[i] = 0;
    }
}
