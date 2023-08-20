//
// Created by pyrowildx on 03/08/2023.
//

#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL_mixer.h>

#define MIX_CHANNEL_N 64

class Sound {

public:
    static void initSound();

    static void cleanUp();

    static void onGameFrame();

    static int playAudioChunk(const char *chunkPath, int repeatCount = 0);

    static void deleteAudioChunk(int channel);

    static Mix_Music *playAudioMusic(const char *musicPath, int repeatCount = 0);

    static void deleteAudioMusic();

private:
    static Mix_Chunk *chunkArray[MIX_CHANNEL_N];
    static Mix_Music *currentAudioMusic;

};

#endif
