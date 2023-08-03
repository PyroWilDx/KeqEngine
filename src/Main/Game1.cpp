//
// Created by pyrowildx on 09/07/2023.
//

#include "Main/Game1.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "Keqing.hpp"
#include "Utils/Events.hpp"
#include "Entity/Slime.hpp"
#include "Entity/FPSText.hpp"

void runGame1() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    World *gWorld = Global::setWorld(SCREEN_BASE_WIDTH, SCREEN_BASE_HEIGHT,
                                     3000, 720,
                                     "res/gfx/background/Game1.png");

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
    gWorld->setRenderKeqing(true);
    gWorld->setTranslateEntity(kq);

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

    auto *slime = new Slime("Blue");
    slime->setHitbox({1, 4, 14, 12});
    slime->setRenderWHMultiplier(4.0, 4.0);
    slime->moveToDownLeft(400, 720 - 200);
    gWorld->addMonster(slime);

    auto *gFPSText = new FPSText();
    gWorld->addOtherEntity(gFPSText);

    SDL_Event event;
    gStateInfo gInfo = DEFAULT_GAME_STATE_INFO;

    while (gInfo.gRunning) {

        handleTime();

        // Events
        int key = KEY_UNDEFINED;
        while (SDL_PollEvent(&event)) {
            handleBasicEvents(&event, &key, &gInfo);
        }
        if (!gInfo.gRunning) break;

        // Dev Mode
        if (gInfo.gPaused) {
            if (!gInfo.runFrame) continue;
            else gInfo.runFrame = false;
        }

        // World
        gWorld->onGameFrame();

        // Window Rendering
        gWindow->clear();

        gWorld->renderSelf();

        gWorld->renderDebugMode();

        gWindow->display();
    }
}
