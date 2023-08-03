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

Mix_Chunk *Global::currentAudioChunk = nullptr;
Mix_Music *Global::currentAudioMusic = nullptr;

void Global::initGlobal() {
    for (int i = 0; i < KEY_ENUM_N; i++) {
        pressedKeys[i] = false;
        pressedTime[i] = 0;
        lastPressedTime[i] = 0;
    }
}

void Global::cleanUp() {
    deleteWorld();
    deleteAudioChunk();
    deleteAudioMusic();
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

Mix_Chunk *Global::setAudioChunk(const char *chunkPath) {
    if (currentAudioChunk != nullptr) deleteAudioChunk();

    currentAudioChunk = Mix_LoadWAV(chunkPath);
    return currentAudioChunk;
}

void Global::deleteAudioChunk() {
    if (currentAudioChunk != nullptr) {
        Mix_FreeChunk(currentAudioChunk);
        currentAudioChunk = nullptr;
    }
}

Mix_Music *Global::setAudioMusic(const char *musicPath) {
    if (currentAudioMusic != nullptr) deleteAudioMusic();

    currentAudioMusic = Mix_LoadMUS(musicPath);
    return currentAudioMusic;
}

void Global::deleteAudioMusic() {
    if (currentAudioMusic != nullptr) {
        Mix_FreeMusic(currentAudioMusic);
        currentAudioMusic = nullptr;
    }
}
