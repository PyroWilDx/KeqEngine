//
// Created by pyrowildx on 09/07/2023.
//

#include <cstdio>
#include "Main/Game1.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Events.hpp"

void runGame1() {
    const char *fontPath = "res/fonts/JetBrainsMono-Regular.ttf";
    const SDL_Color FPSTextColor = {RGBA_FULL};
    const int FPSFontSize = 40;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    auto *gWorld = new World(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             3000, 720,
                             "res/gfx/background/Background.png");
    Global::currentWorld = gWorld;
    gWorld->addBlock(BLOCK_DIRT,
                     300, 720 - 200, 176, 500);
    gWorld->addBlock(BLOCK_DIRT,
                     751, 720 - 200, 176, 200);
    gWorld->addBlock(BLOCK_DIRT,
                     1000, 720 - 20, 176, 20);
    gWorld->addBlock(BLOCK_DIRT,
                     1120, 720 - 120, 176, 40);
    gWorld->addBlock(BLOCK_DIRT,
                     1200, 720 - 180, 176, 40);

    Keqing *kq = Keqing::getInstance();
    kq->moveTo(0, 0);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    kq->setHitbox({(int) (0.0 * KQ_WIDTH_MULTIPLIER),
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

    bool gRunning = true;
    bool gPaused = false;
    bool runFrame = false;

    while (gRunning) {

        handleTime();

        // Events
        int SDLKey;
        int key;
        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &gRunning);

            switch (event.type) {
                case SDL_KEYDOWN:
                    SDLKey = event.key.keysym.sym;
                    switch (SDLKey) {
                        case SDLK_BACKSPACE:
                            gPaused = !gPaused;
                            if (!gPaused) Global::currentTime = getTime();
                            break;
                        case SDLK_RETURN:
                            if (gPaused) {
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

        if (gPaused) {
            if (!runFrame) continue;
            else runFrame = false;
        }

        int spriteCode = KQ_IDLE;
        if (key != -1) { // There is a SDLKey event
            if (isKeyPressed(KEY_R)) {
                spriteCode = KQ_BURST;

            } else if (isKeyPressed(KEY_E)) {
                if (!Keqing::isLightningStilettoExisting()) spriteCode = KQ_SKILL;
                else spriteCode = KQ_SKILL_SLASH;

            } else if (isKeyPressed(KEY_SHIFT)) {
                if (!kq->isInAir()) {
                    if (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) {
                        if (isKeyPressedShort(KEY_SHIFT)) {
                            spriteCode = KQ_DASH;
                        }
                    }
                } else {
                    spriteCode = KQ_AIR_DASH;
                }

            } else if (isKeyPressed(KEY_MOUSE_LEFT)) {
                if (!kq->isInAir()) {
                    spriteCode = KQ_NATK;
                } else {
                    if (isKeyPressed(KEY_S)) {
                        spriteCode = KQ_AIR_NATK;
                    }
                }

            } else if (isKeyPressed(KEY_SPACE)) {
                if (!kq->isInAir()) {
                    spriteCode = KQ_JUMP_START;
                }

            } else if (isKeyPressed(KEY_Q) ||
                       isKeyPressed(KEY_D)) {
                if (!isKeyPressed(KEY_MOUSE_RIGHT)) {
                    spriteCode = KQ_WALK;
                } else {
                    spriteCode = KQ_RUN_START;
                }

            }
        }

        if (kq->canDoAction(spriteCode)) {
            kq->setSpriteAnimated(true, spriteCode);
        }

        // Particles
        Particle::animateAll();

        // Keqing
        // TODO Hitlag
        kq->fallGravity();

        if (kq->shouldUpdateDirection()) kq->updateDirection();
        if (kq->canMoveLR()) kq->moveLR();

        kq->updateAction();

        kq->moveX();
        kq->moveY();

        kqX = kq->getX();
        double xDiff = kqX - kqLastX;
        if (xDiff != 0) gWorld->getBackground()->translate(kq);
        kqLastX = kqX;

        kq->airAnimate();
        kq->animateSprite();

        // FPS Text
        if (accumulatedFPSTime > 1000) {
            char text[16];
            sprintf(text, "FPS : %d", accumulatedFrames);
            FPSText.loadTextTexture(0, 0, text, &FPSTextColor,
                                    fontPath, FPSFontSize, false);
            accumulatedFPSTime = 0;
            accumulatedFrames = 0;
        }
        accumulatedFPSTime += Global::dt;
        accumulatedFrames++;

        // Window Rendering
        gWindow->clear();

        gWorld->renderSelf();

        gWindow->renderEntity(&FPSText);

        gWindow->renderEntity(kq);

        Particle::renderAll();

        gWindow->display();
    }

    delete gWorld;
}
