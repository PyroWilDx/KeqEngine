#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include "Main/HomeMenu.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"

int main(int argc, char *argv[]) {
    std::string path = std::filesystem::current_path().string();
    std::filesystem::current_path(path + "/..");

    myAssert(SDL_Init(SDL_INIT_VIDEO) >= 0, "SDL_Init FAILED.", SDL_GetError());
    myAssert(IMG_Init(IMG_INIT_PNG) != 0, "IMG_Init FAILED.", SDL_GetError());
    myAssert(TTF_Init() != -1, "TTF_Init FAILED.", SDL_GetError());

    WindowRenderer::initWindowRenderer("Keqing", SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT);
    Global::initGlobal();
    Keqing::initKeqing();
    Particle::initParticle();

    runHomeMenu();

    Particle::cleanUp();
    WindowRenderer::getInstance()->cleanUp();
    SDL_Quit();

    return EXIT_SUCCESS;
}
