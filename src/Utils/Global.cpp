//
// Created by pyrowildx on 25/06/2023.
//

#include <fstream>
#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"

std::unordered_map<std::string, std::string> Global::userData;

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
    readUserData();

    for (int i = 0; i < KEY_ENUM_N; i++) {
        pressedKeys[i] = false;
        pressedTime[i] = 0;
        lastPressedTime[i] = 0;
    }
}

void Global::cleanUp() {
    deleteWorld();
}

void Global::readUserData() {
    std::ifstream file;
    file.open(DATA_PATH);
    myAssert(file.is_open(), "Error Opening Data File (For Read).");

    std::string keyStr, valStr;
    while (std::getline(file, keyStr, '=')) {
        std::getline(file, valStr);
        userData[keyStr] = valStr;
    }

    file.close();
}

void Global::saveUserData(const char *key, std::string &valStr) {
    std::string keyStr = std::string(key);
    userData[keyStr] = valStr;

    std::fstream file;
    file.open(DATA_PATH, std::ios::in);
    myAssert(file.is_open(), "Error Opening Data File (For Save).");

    std::string finalStr = std::string("");
    std::string readKeyStr, readValStr;
    bool foundKey = false;
    while (std::getline(file, readKeyStr, '=')) {
        finalStr += readKeyStr + "=";
        if (keyStr == readKeyStr) {
            foundKey = true;
            finalStr += valStr;
            std::getline(file, readValStr);
        } else {
            std::getline(file, readValStr);
            finalStr += readValStr;
        }
        finalStr += "\n";
    }

    file.close();

    if (!foundKey) {
        SDL_Log("Couldn't Find the Key to Save in Data.");
        return;
    }

    file.open(DATA_PATH, std::ofstream::out | std::ofstream::trunc);
    myAssert(file.is_open(), "Error Opening Data File (For Save).");

    file.write(finalStr.c_str(), finalStr.size());

    file.close();
}

World *Global::setWorld(int screenW, int screenH,
                        int backgroundTotalW, int backgroundTotalH,
                        const char *backgroundImgPath) {
    if (currentWorld != nullptr) deleteWorld();

    currentWorld = new World(screenW, screenH,
                             backgroundTotalW, backgroundTotalH,
                             backgroundImgPath);
    return currentWorld;
}

void Global::deleteWorld() {
    if (currentWorld != nullptr) {
        delete currentWorld;
        currentWorld = nullptr;
    }
}
