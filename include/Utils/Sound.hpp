//
// Created by pyrow on 03/08/2023.
//

#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL_mixer.h>

Mix_Chunk *playAudioChunk(const char *chunkPath, int repeatCount = 0);

Mix_Music *playAudioMusic(const char *musicPath, int repeatCount = 0);

#endif
