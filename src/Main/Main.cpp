#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <filesystem>
#include <iostream>
#include "Main/HomeMenu.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Entity/Keqing.hpp"
#include "EntityRes/Sound.hpp"

int main(int argc, char *argv[]) {
    std::string path = std::filesystem::current_path().string();
    std::filesystem::current_path(path + "/..");

    myAssert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0,
             "SDL_Init FAILED.", SDL_GetError());
    int imgFlags = IMG_INIT_PNG;
    myAssert(IMG_Init(imgFlags) & imgFlags,
             "IMG_Init FAILED.", IMG_GetError());
    myAssert(TTF_Init() != -1,
             "TTF_Init FAILED.", TTF_GetError());
    myAssert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,
                           2, 2048) >= 0,
             "Mix_OpenAudio FAILED.", Mix_GetError());
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

    WindowRenderer::initWindowRenderer("Keqing", SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT);
    Global::initGlobal();
    Sound::initSound();
    Keqing::initKeqing();
    Particle::initParticle();

    MainHomeMenu::Run();

    Particle::cleanUp();
    Keqing::cleanUp();
    Sound::cleanUp();
    Global::cleanUp();
    WindowRenderer::cleanUp();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
