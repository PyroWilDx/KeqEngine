//
// Created by pyrowildx on 25/06/2023.
//

#include <fstream>
#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"
#include "World/World.hpp"

std::unordered_map<std::string, std::string> Global::userData;
sqlite3 *Global::db;

int Global::windowWidth;
int Global::windowHeight;
int Global::renderWindowWidth;
int Global::renderWindowHeight;
bool Global::isWindowFullscreen;

bool Global::pressedKeys[KEY_ENUM_N];
int Global::pressedTime[KEY_ENUM_N];
int Global::lastPressedTime[KEY_ENUM_N];

int Global::currentTime;
int Global::dt;

World *Global::currentWorld;

void Global::initGlobal() {
    readUserData();
    myAssert(sqlite3_open(DB_PATH, &db) == SQLITE_OK, "SQLITE3_Open FAILED.");

    windowWidth = SCREEN_BASE_WIDTH;
    windowHeight = SCREEN_BASE_HEIGHT;
    renderWindowWidth = SCREEN_BASE_WIDTH;
    renderWindowHeight = SCREEN_BASE_HEIGHT;
    isWindowFullscreen = false;

    for (int i = 0; i < KEY_ENUM_N; i++) {
        pressedKeys[i] = false;
        pressedTime[i] = 0;
        lastPressedTime[i] = 0;
    }

    currentTime = getTime();
    dt = 10;

    currentWorld = nullptr;
}

void Global::cleanUp() {
    deleteWorld();

    sqlite3_close(db);
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
