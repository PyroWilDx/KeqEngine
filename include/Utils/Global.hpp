//
// Created by pyrowildx on 25/06/2023.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL_mixer.h>
#include "World/World.hpp"
#include "Utils/Events.hpp"

class Global {

public:
    Global() = delete;

    static void initGlobal();

    static void cleanUp();

    [[nodiscard]] static World *setWorld(int screenW, int screenH,
                                         int backgroundTotalW, int backgroundTotalH,
                                         const char *backgroundImgPath);

    static void deleteWorld();

    [[nodiscard]] static Mix_Chunk *setAudioChunk(const char *chunkPath);

    static void deleteAudioChunk();

    [[nodiscard]] static Mix_Music *setAudioMusic(const char *musicPath);

    static void deleteAudioMusic();

    static int windowWidth;
    static int windowHeight;
    static int renderWindowWidth;
    static int renderWindowHeight;
    static bool isWindowFullscreen;

    static bool pressedKeys[KEY_ENUM_N];
    static int pressedTime[KEY_ENUM_N];
    static int lastPressedTime[KEY_ENUM_N];

    static int currentTime;
    static int dt;

    static World *currentWorld;

    static Mix_Chunk *currentAudioChunk;
    static Mix_Music *currentAudioMusic;

private:

};

#endif
