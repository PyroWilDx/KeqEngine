//
// Created by pyrowildx on 13/05/23.
//

#ifdef __unix__

#include <gperftools/profiler.h>

#endif

#include <filesystem>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Main/HomeMenu.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Sound.hpp"
#include "Utils/Random.hpp"
#include "Utils/Utils.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Draw.hpp"
#include "Utils/Colors.hpp"

int main(int argc, char *argv[]) {

#ifdef __unix__
//    ProfilerStart("Profiler.prof");
#endif

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

    WindowRenderer::initWindowRenderer("Keqing", SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT);
    Global::initGlobal();
    Sound::initSound();
    Colors::initColors();
    Draw::initDraw();
    Events::initEvents();
    Random::initRandom();
    Keqing::initKeqing();
    Particle::initParticle();

    HomeMenu::Run();

    Particle::cleanUp();
    Keqing::cleanUp();
    Random::cleanUp();
    Events::cleanUp();
    Draw::cleanUp();
    Colors::cleanUp();
    Sound::cleanUp();
    Global::cleanUp();
    WindowRenderer::cleanUp();

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

#ifdef __unix__
    ProfilerStop();
#endif

    return EXIT_SUCCESS;
}
