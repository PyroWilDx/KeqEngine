//
// Created by pyrowildx on 09/07/2023.
//

#include <cstdio>
#include "Main/Game1.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Events.hpp"
#include "Entity/Slime.hpp"

void runGame1() {
    const char *fontPath = "res/fonts/JetBrainsMono-Regular.ttf";
    const SDL_Color FPSTextColor = {RGBA_FULL};
    const int FPSFontSize = 40;

    WindowRenderer *gWindow = WindowRenderer::getInstance();

    auto *gWorld = new World(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                             3000, 720,
                             "res/gfx/background/Game1.png");
    Global::currentWorld = gWorld;
    gWorld->addBlock(BLOCK_DIRT,
                     300, 720 - 200, 400, 500);
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
    kq->setHitbox({0, 12, 60, 84});
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);

    const int hudSBCircleY = SCREEN_BASE_HEIGHT - 130;
    const int hudSBCircleBGAlpha = 128;

    const int hudSkillCircleX = 1016;
    Particle *skillCircleBG =
            Particle::pushParticle(PARTICLE_HUD_SKILL_CIRCLE_BG, INT32_MAX,
                                   HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    skillCircleBG->moveTo(hudSkillCircleX, hudSBCircleY);
    skillCircleBG->setRGBAMod(RGB_FULL, hudSBCircleBGAlpha);

    Particle *skillCircle =
            Particle::pushParticle(PARTICLE_HUD_SKILL_CIRCLE, INT32_MAX,
                                   HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    skillCircle->moveToEntityCenter(skillCircleBG);
    skillCircle->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);

    Particle *burstCircleBG =
            Particle::pushParticle(PARTICLE_HUD_BURST_CIRCLE_BG, INT32_MAX,
                                   HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    double hudSCW;
    skillCircle->getRealSize(&hudSCW, nullptr);
    burstCircleBG->moveTo(hudSkillCircleX + hudSCW,
                          hudSBCircleY);
    burstCircleBG->setRGBAMod(RGB_FULL, hudSBCircleBGAlpha);

    Particle *burstCircle =
            Particle::pushParticle(PARTICLE_HUD_BURST_CIRCLE, INT32_MAX,
                                   HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
    burstCircle->moveToEntityCenter(burstCircleBG);

    Particle *skillIcon1 =
            Particle::pushParticle(PARTICLE_HUD_SKILL_ICON_1, INT32_MAX,
                                   HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon1->moveToEntityCenter(skillCircle);

    Particle *burstIcon =
            Particle::pushParticle(PARTICLE_HUD_BURST_ICON, INT32_MAX,
                                   HUB_SB_ICON_M, HUB_SB_ICON_M);
    burstIcon->moveToEntityCenter(burstCircle);

    // TODO AD DIN WORLD
    auto *slime = new Slime("Blue");
    slime->setHitbox({1, 4, 14, 12});
    slime->setRenderWHMultiplier(4.0, 4.0);
    slime->moveToDownLeft(320, 720 - 200);
    gWorld->addMonster(slime);

    SDL_Event event;

    double kqLastX = kq->getX();
    double kqX;

    Text FPSText = Text();
    int accumulatedFPSTime = 10000;
    int accumulatedFrames = 0;

    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    while (gInfo.gRunning) {

        handleTime();

        // Events
        int key = KEY_UNDEFINED;
        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &key, &gInfo);
        }

        // Dev Mode
        if (gInfo.gPaused) {
            if (!gInfo.runFrame) continue;
            else gInfo.runFrame = false;
        }

        // Handling Keys
        int spriteCode = KQ_IDLE;

        if (isKeyPressed(KEY_R)) {
            if (!kq->isInAir()) {
                spriteCode = KQ_BURST;
            }

        } else if (isKeyPressed(KEY_E)) {
            if (!kq->isInAir()) {
                if (!Keqing::isLightningStilettoExisting()) {
                    spriteCode = KQ_SKILL;
                } else {
                    spriteCode = KQ_SKILL_SLASH;
                }
            }

        } else if (isKeyPressed(KEY_SHIFT)) {
            if (kq->isInAir()) {
                spriteCode = KQ_AIR_DASH;
            } else {
                if (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) {
                    if (isKeyPressedShort(KEY_SHIFT)) {
                        spriteCode = KQ_DASH;
                    }
                }
            }

        } else if (isKeyPressed(KEY_MOUSE_LEFT)) {
            if (kq->isInAir()) {
                if (isKeyPressed(KEY_S)) {
                    spriteCode = KQ_AIR_PLUNGE;
                } else if (isKeyPressed(KEY_Z)) {
                    spriteCode = KQ_AIR_UP_NATK;
                } else {
                    spriteCode = KQ_AIR_NATK;
                }
            } else {
                if (isKeyPressed(KEY_Z)) {
                    spriteCode = KQ_UP_NATK;
                } else if (isKeyPressed(KEY_S)) {
                    spriteCode = KQ_CROUCH_NATK;
                } else {
                    spriteCode = KQ_NATK;
                }
            }

        } else if (isKeyPressed(KEY_SPACE)) {
            if (kq->isInAir()) {
                spriteCode = KQ_AIR_DOUBLE_JUMP;
            } else {
                spriteCode = KQ_JUMP_START;
            }

        } else if (isKeyPressed(KEY_S)) {
            if (!kq->isInAir()) {
                spriteCode = KQ_CROUCH;
            }

        } else if (isKeyPressed(KEY_Q) ||
                   isKeyPressed(KEY_D)) {
            if (!isKeyPressed(KEY_MOUSE_RIGHT)) {
                spriteCode = KQ_WALK;
            } else {
                spriteCode = KQ_RUN;
            }

        }

        // Keqing Action
        if (kq->canDoAction(spriteCode)) {
            kq->setSpriteAnimated(true, spriteCode);
        }

        // Particles
        Particle::animateAll();

        // World
        gWorld->onGameFrame();

        // Keqing
        // TODO Hitlag
        kq->fallGravity();

        if (kq->shouldUpdateDirection()) kq->updateDirection();
        if (kq->canMoveLR()) kq->moveLR();

        kq->updateAction();

        kq->moveX();
        kq->moveY();

        kq->airAnimate();
        kq->animateSprite();

        // Background Translation
        kqX = kq->getX();
        double xDiff = kqX - kqLastX;
        if (xDiff != 0) gWorld->getBackground()->translate(kq);
        kqLastX = kqX;

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

        // TODO Maybe put this in world ???? do a vector of other entities to render ??
        gWindow->renderEntity(&FPSText);

        gWindow->renderEntity(kq);

        Particle::renderAll();

        gWorld->renderDebugMode();

        gWindow->display();
    }

    delete gWorld;
}
