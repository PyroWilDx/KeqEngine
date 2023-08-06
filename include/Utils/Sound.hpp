//
// Created by pyrow on 03/08/2023.
//

#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL_mixer.h>

class Sound {

public:
    static void initSound();

    static void cleanUp();

    static Mix_Chunk *playAudioChunk(const char *chunkPath, int repeatCount = 0);

    static Mix_Music *playAudioMusic(const char *musicPath, int repeatCount = 0);

    [[nodiscard]] static Mix_Chunk *setAudioChunk(const char *chunkPath);

    static void deleteAudioChunk();

    [[nodiscard]] static Mix_Music *setAudioMusic(const char *musicPath);

    static void deleteAudioMusic();

    static Mix_Chunk *currentAudioChunk;
    static Mix_Music *currentAudioMusic;

private:

};

#endif
