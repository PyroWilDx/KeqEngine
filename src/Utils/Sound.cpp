//
// Created by pyrow on 03/08/2023.
//

#include "Utils/Sound.hpp"
#include "Utils/Global.hpp"

Mix_Chunk *Sound::currentAudioChunk = nullptr;
Mix_Music *Sound::currentAudioMusic = nullptr;

void Sound::initSound() {
    currentAudioChunk = nullptr;
    currentAudioMusic = nullptr;
}

void Sound::cleanUp() {
    Sound::deleteAudioChunk();
    Sound::deleteAudioMusic();
}

Mix_Chunk *Sound::playAudioChunk(const char *chunkPath, int repeatCount) {
    Mix_Chunk *currChunk = Sound::setAudioChunk(chunkPath);
    Mix_PlayChannel(-1, currChunk, repeatCount);
    return currChunk;
}

Mix_Music *Sound::playAudioMusic(const char *musicPath, int repeatCount) {
    Mix_Music *currMusic = Sound::setAudioMusic(musicPath);
    Mix_PlayMusic(currMusic, repeatCount);
    return currMusic;
}

Mix_Chunk *Sound::setAudioChunk(const char *chunkPath) {
    if (currentAudioChunk != nullptr) Sound::deleteAudioChunk();

    currentAudioChunk = Mix_LoadWAV(chunkPath);
    return currentAudioChunk;
}

void Sound::deleteAudioChunk() {
    if (currentAudioChunk != nullptr) {
        Mix_FreeChunk(currentAudioChunk);
        currentAudioChunk = nullptr;
    }
}

Mix_Music *Sound::setAudioMusic(const char *musicPath) {
    if (currentAudioMusic != nullptr) Sound::deleteAudioMusic();

    currentAudioMusic = Mix_LoadMUS(musicPath);
    return currentAudioMusic;
}

void Sound::deleteAudioMusic() {
    if (currentAudioMusic != nullptr) {
        Mix_FreeMusic(currentAudioMusic);
        currentAudioMusic = nullptr;
    }
}
