#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Player.hpp"
#include "Text.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Text constants
const char *fontPath = "res/fonts/JetBrainsMono-Regular.ttf";
const SDL_Color FPSTextColor = {255, 255, 255};
const int FPSFontSize = 40;

int main() {
    std::string path = std::filesystem::current_path();
    std::filesystem::current_path(path + "/..");

    myAssert(SDL_Init(SDL_INIT_VIDEO) >= 0, "SDL_Init FAILED.", SDL_GetError());
    myAssert(IMG_Init(IMG_INIT_PNG) != 0, "IMG_Init FAILED.", SDL_GetError());
    myAssert(TTF_Init() != -1, "TTF_Init FAILED.", SDL_GetError());

    WindowRenderer window = WindowRenderer("FirstSDL2", SCREEN_WIDTH, SCREEN_HEIGHT);

    Player player = Player(0, SCREEN_HEIGHT - 160, 0, 80, 80, window);
    player.setRenderWH(160, 160);

    Uint32 dt;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;

    Uint32 accumulatedAnimationTime = 10000;

    Text FPSText = Text();
    Uint32 accumulatedFPSTime = 10000;
    Uint32 accumulatedFrames = 0;

    SDL_Event event;
    int key;
    bool keyPressed[4] = {false, false, false, false};

    bool gameRunning = true;
    while (gameRunning) {

        // Events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    key = event.key.keysym.sym;
                    switch (key) {
                        case SDLK_q:
                        case SDLK_d:
                        case SDLK_s:
                        case SDLK_z:
                            keyPressed[key % 4] = true;
                            player.updateDirection(key, keyPressed);
                            break;
                        case SDLK_SPACE:
                            player.jump();
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    key = event.key.keysym.sym;
                    switch (key) {
                        case SDLK_q:
                        case SDLK_d:
                        case SDLK_s:
                        case SDLK_z:
                            keyPressed[key % 4] = false;
                            player.clearDirection(key, keyPressed);
                            break;
                        case SDLK_SPACE:
                            player.jump();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // Move Player
        if (player.isMoving()) player.move((int) dt);

        // Time Handling
        currentTime = SDL_GetTicks();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        // Player Animation
        player.updateTexture();
        player.animate(&accumulatedAnimationTime);
        accumulatedAnimationTime += dt;

        // FPS Text
        if (accumulatedFPSTime > 1000) {
            char text[16];
            sprintf(text, "FPS : %d", accumulatedFrames);
            FPSText.loadTextTexture(0, 0, text, &FPSTextColor,
                                    fontPath, FPSFontSize, window.getRenderer());
            accumulatedFPSTime = 0;
            accumulatedFrames = 0;
        }
        accumulatedFPSTime += dt;
        accumulatedFrames++;

        window.clear();
        window.render(&player);
        window.render(&FPSText);
        window.display();
    }

    // Free
    FPSText.destroy();
    player.destroy();
    window.cleanUp();
    SDL_Quit();

    return EXIT_SUCCESS;
}