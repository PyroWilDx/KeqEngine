//
// Created by pyrow on 03/08/2023.
//

#include "Utils/Sound.hpp"
#include "Utils/Global.hpp"

Mix_Chunk *playAudioChunk(const char *chunkPath, int repeatCount) {
    Mix_Chunk *currChunk = Global::setAudioChunk(chunkPath);
    Mix_PlayChannel(-1, currChunk, repeatCount);
    return currChunk;
}

Mix_Music *playAudioMusic(const char *musicPath, int repeatCount) {
    Mix_Music *currMusic = Global::setAudioMusic(musicPath);
    Mix_PlayMusic(currMusic, repeatCount);
    return currMusic;
}
