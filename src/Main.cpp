#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Keqing.hpp"
#include "Text.hpp"
#include "Particle.hpp"
#include "Global.hpp"

// Text constants
const char *fontPath = "res/fonts/JetBrainsMono-Regular.ttf";
const SDL_Color FPSTextColor = {RGBA_FULL};
const int FPSFontSize = 40;

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

    WindowRenderer *gWindow = WindowRenderer::getInstance();
    SDL_SetWindowResizable(gWindow->getWindow(), SDL_TRUE);

    Global::currentWorld = new World(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 720,
                                     "res/gfx/Background.png");
    Global::currentWorld->addBlock(BLOCK_DIRT,
                                   300, 720-200, 176, 500);
    Global::currentWorld->addBlock(BLOCK_DIRT,
                                   751, 720-200, 176, 200);
    Global::currentWorld->addBlock(BLOCK_DIRT,
                                   1000, 720-20, 176, 20);
    Global::currentWorld->addBlock(BLOCK_DIRT,
                                   1120, 720-120, 176, 40);

    Keqing *kq = Keqing::getInstance();
    kq->moveTo(0, 360);
//    kq->colorTexture(4, 1, 4); // TODO
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    kq->getHitbox({(int) (0.0 * KQ_WIDTH_MULTIPLIER),
                   (int) (12.0 * KQ_HEIGHT_MULTIPLIER),
                   (int) (60.0 * KQ_WIDTH_MULTIPLIER),
                   (int) (84.0 * KQ_HEIGHT_MULTIPLIER)});

    const int hudSBCircleY = SCREEN_BASE_HEIGHT - 130;
    const int hudSBCircleBGAlpha = 128;

    const int hudSkillCircleX = 1016;
    Particle *skillCircleBG =
            Particle::push(PARTICLE_HUD_SKILL_CIRCLE_BG, INT32_MAX,
                           HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    skillCircleBG->moveTo(hudSkillCircleX, hudSBCircleY);
    skillCircleBG->setRGBAMod(RGB_FULL, hudSBCircleBGAlpha);

    Particle *skillCircle =
            Particle::push(PARTICLE_HUD_SKILL_CIRCLE, INT32_MAX,
                           HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    skillCircle->moveToEntityCenter(skillCircleBG);
    skillCircle->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);

    Particle *burstCircleBG =
            Particle::push(PARTICLE_HUD_BURST_CIRCLE_BG, INT32_MAX,
                           HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    double hudSCW;
    skillCircle->getRealSize(&hudSCW, nullptr);
    burstCircleBG->moveTo(hudSkillCircleX + hudSCW,
                          hudSBCircleY);
    burstCircleBG->setRGBAMod(RGB_FULL, hudSBCircleBGAlpha);

    Particle *burstCircle =
            Particle::push(PARTICLE_HUD_BURST_CIRCLE, INT32_MAX,
                           HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    burstCircle->moveToEntityCenter(burstCircleBG);

    Particle *skillIcon1 =
            Particle::push(PARTICLE_HUD_SKILL_ICON_1, INT32_MAX,
                           HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon1->moveToEntityCenter(skillCircle);

    Particle *burstIcon =
            Particle::push(PARTICLE_HUD_BURST_ICON, INT32_MAX,
                           HUB_SB_ICON_M, HUB_SB_ICON_M);
    burstIcon->moveToEntityCenter(burstCircle);

    SDL_Event event;

    double kqLastX = kq->getX();
    double kqX;

    Text FPSText = Text();
    int accumulatedFPSTime = 10000;
    int accumulatedFrames = 0;

    bool gameRunning = true;
    bool gamePaused = false;
    bool runFrame = false;
    while (gameRunning) {

        // Time Handling
        int currentTime = getTime();
        Global::dt = currentTime - Global::currentTime;
        Global::currentTime = currentTime;

        // Events
        int spriteCode = -1;
        int SDLKey;
        int key;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            Global::windowWidth = event.window.data1;
                            Global::windowHeight = event.window.data2;
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    SDLKey = event.key.keysym.sym;
                    switch (SDLKey) {
                        case SDLK_BACKSPACE:
                            gamePaused = !gamePaused;
                            if (!gamePaused) Global::currentTime = getTime();
                            break;
                        case SDLK_RETURN:
                            if (gamePaused) {
                                runFrame = true;
                                Global::dt = 10;
                            }
                            break;
                        default:
                            key = updatePressedKeys(SDLKey, true, true);
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    SDLKey = event.key.keysym.sym;
                    key = updatePressedKeys(SDLKey, false, true);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    SDLKey = event.button.button;
                    key = updatePressedKeys(SDLKey, true, false);
                    break;

                case SDL_MOUSEBUTTONUP:
                    SDLKey = event.button.button;
                    key = updatePressedKeys(SDLKey, false, false);
                    break;

                default:
                    break;
            }
        }

        if (gamePaused) {
            if (!runFrame) continue;
            else runFrame = false;
        }

        if (key != -1) { // There is a SDLKey event
            if (Global::pressedKeys[KEY_R]) {
                spriteCode = KQ_BURST;

            } else if (Global::pressedKeys[KEY_E]) {
                if (!Keqing::isLightningStilettoExisting()) spriteCode = KQ_SKILL;
                else spriteCode = KQ_SKILL_SLASH;

            } else if (Global::pressedKeys[KEY_SPACE]) {
                spriteCode = KQ_JUMP_START;

            } else if (Global::pressedKeys[KEY_SHIFT]) {
                if (!kq->isJumping()) {
                    if (Global::pressedKeys[KEY_Q] || Global::pressedKeys[KEY_D]) {
                        spriteCode = KQ_DASH;
                    }
                } else {
                    spriteCode = KQ_AIR_DASH;
                }

            } else if (Global::pressedKeys[KEY_MOUSE_LEFT]) {
                if (!kq->isJumping()) spriteCode = KQ_NATK;
                else spriteCode = KQ_AIR_NATK;
            }
        }

        if (spriteCode != -1) {
            if (kq->canDoAction(spriteCode)) {
                kq->preAction(spriteCode);
                kq->setSpriteAnimated(spriteCode, true);
            }
        }

        // Particles
        Particle::animateAll();

        // Keqing
        // TODO Hitlag
        kq->fallGravity();
        if (kq->canDoAction(KQ_WALK)) kq->updateDirection();
        if (kq->isNAtking()) kq->NAtk();
        if (kq->isDashing()) kq->dash();
        if (kq->isESkilling()) kq->ESkill();
        if (kq->isESkillSlashing()) kq->ESkillSlash();
        if (kq->isRBursting()) kq->RBurst();
        if (kq->isJumping()) kq->jump();
        if (kq->isAirNAtking()) kq->airNAtk();
        if (kq->isAirDashing()) kq->airDash();
        if (kq->isDamaged()) kq->damage();
        if (kq->isMoving()) kq->moveX(); // TODO
        kq->moveY();

        kqX = kq->getX();
        double xDiff = kqX - kqLastX;
        if (xDiff != 0) Global::currentWorld->getBackground()->translate(kq);
        kqLastX = kqX;

        kq->airAnimate();
        kq->animate();

        // FPS Text
        if (accumulatedFPSTime > 1000) {
            char text[16];
            sprintf(text, "FPS : %d", accumulatedFrames);
            FPSText.loadTextTexture(0, 0, text, &FPSTextColor,
                                    fontPath, FPSFontSize, gWindow->getRenderer());
            accumulatedFPSTime = 0;
            accumulatedFrames = 0;
        }
        accumulatedFPSTime += Global::dt;
        accumulatedFrames++;

        // Window Rendering
        gWindow->clear();

        Global::currentWorld->renderSelf();

        gWindow->render(&FPSText);

        gWindow->render(kq);

        Particle::renderAll();

        gWindow->display();
    }

    // Free
    Particle::cleanUp();
    gWindow->cleanUp();
    SDL_Quit();

    return EXIT_SUCCESS;
}
