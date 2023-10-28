//
// Created by pyrowildx on 03/08/2023.
//

#include "Utils/Sound.hpp"
#include "Utils/Global.hpp"

Mix_Chunk *Sound::chunkArray[MIX_CHANNEL_N];
Mix_Music *Sound::currentAudioMusic = nullptr;

void Sound::initSound() {
    Mix_AllocateChannels(MIX_CHANNEL_N);

    double volumePercent = std::stod(Global::userData[DATA_GAME_VOLUME]);
    int volume = (int) (MIX_MAX_VOLUME * (volumePercent / 100.));
    Mix_Volume(-1, volume);

    for (int i = 0; i < MIX_CHANNEL_N; i++) {
        chunkArray[i] = nullptr;
    }
    currentAudioMusic = nullptr;
}

void Sound::cleanUp() {
    for (int i = 0; i < MIX_CHANNEL_N; i++) {
        Sound::deleteAudioChunk(i);
    }
    Sound::deleteAudioMusic();
}

void Sound::onGameFrame() {
    for (int i = 0; i < MIX_CHANNEL_N; i++) {
        if (chunkArray[i] != nullptr) {
            if (Mix_Playing(i) == 0) {
                deleteAudioChunk(i);
            }
        }
    }
}

int Sound::playAudioChunk(const char *chunkPath, int repeatCount) {
    Mix_Chunk *currChunk = Mix_LoadWAV(chunkPath);
    int usedChannel = -1;
    if (currChunk != nullptr) {
        usedChannel = Mix_PlayChannel(-1, currChunk, repeatCount);
        chunkArray[usedChannel] = currChunk;
    }
    return usedChannel;
}

void Sound::deleteAudioChunk(int channel) {
    if (chunkArray[channel] != nullptr) {
        Mix_FreeChunk(chunkArray[channel]);
        chunkArray[channel] = nullptr;
    }
}

void Sound::stopAllAudioChunks() {
    for (int i = 0; i < MIX_CHANNEL_N; i++) {
        deleteAudioChunk(i);
    }
}

Mix_Music *Sound::playAudioMusic(const char *musicPath, int repeatCount) {
    if (currentAudioMusic != nullptr) Sound::deleteAudioMusic();
    Mix_Music *currMusic = Mix_LoadMUS(musicPath);
    if (currMusic != nullptr) {
        Mix_PlayMusic(currMusic, repeatCount);
        currentAudioMusic = Mix_LoadMUS(musicPath);
    }
    return currMusic;
}

void Sound::deleteAudioMusic() {
    if (currentAudioMusic != nullptr) {
        Mix_FreeMusic(currentAudioMusic);
        currentAudioMusic = nullptr;
    }
}

void Sound::stopAllSounds() {
    stopAllAudioChunks();
    deleteAudioMusic();
}
