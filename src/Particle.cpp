//
// Created by pyrowildx on 06/06/23.
//

#include "Particle.hpp"
#include "Keqing.hpp"

Sprite Particle::allParticleTextures[PARTICLE_ENUM_N];

int Particle::activeParticleMaxes[PARTICLE_ENUM_N];
Particle **Particle::activeParticles[PARTICLE_ENUM_N];
int Particle::counts[PARTICLE_ENUM_N];

Particle::Particle(int spriteCode, int xShift, int yShift, int frameDuration,
                   float wMultiplier, float hMultiplier, Entity *entity)
        : AnimatedEntity(1), fadeParams({-1, 1.0f}) {
    spriteArray[0] = allParticleTextures[spriteCode];
    Sprite *sprite = &spriteArray[0];
    sprite->animated = true;
    sprite->frameDuration = frameDuration;
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    this->entity = entity;
    entityDependant = true;
    facingEast = entity->isFacingEast();
    stopOnLastFrame = false;
    nextParticle = nullptr;
    frame.w = sprite->width;
    frame.h = sprite->height;
    this->xShift = xShift;
    this->yShift = yShift;
    this->xShiftR = -xShift;
}

void Particle::initParticle(WindowRenderer *window) {
    allParticleTextures[PARTICLE_KQ_NATK_4] =
            {PARTICLE_KQ_NATK_4, false,
             window->loadTexture("res/gfx/particle/KQNAtk4.png"),
             0, 0, 0,
             80, 32,
             6 * 80, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_NATK_4] = 1;

    allParticleTextures[PARTICLE_KQ_AIR_NATK] =
            {PARTICLE_KQ_AIR_NATK, false,
             window->loadTexture("res/gfx/particle/KQAirNAtk.png"),
             0, 0, 0,
             32, 80,
             4 * 32, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_AIR_NATK] = 1;

    allParticleTextures[PARTICLE_KQ_AIR_NATK_GROUND] =
            {PARTICLE_KQ_AIR_NATK_GROUND, false,
             window->loadTexture("res/gfx/particle/KQAirNAtkGround.png"),
             0, 0, 0,
             192, 96,
             6 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_AIR_NATK_GROUND] = 1;

    allParticleTextures[PARTICLE_KQ_SKILL_SPAWN] =
            {PARTICLE_KQ_SKILL_SPAWN, false,
             window->loadTexture("res/gfx/particle/KQSkillSpawn.png"),
             0, 0, 0,
             96, 96,
             8 * 96, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SKILL_SPAWN] = 1;

    allParticleTextures[PARTICLE_KQ_SKILL_IDLE] =
            {PARTICLE_KQ_SKILL_IDLE, false,
             window->loadTexture("res/gfx/particle/KQSkillIdle.png"),
             0, 0, 0,
             32, 32,
             4 * 32, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SKILL_IDLE] = 1;

    allParticleTextures[PARTICLE_KQ_SKILL_TP_END] =
            {PARTICLE_KQ_SKILL_TP_END, false,
             window->loadTexture("res/gfx/particle/KQSkillTPEnd.png"),
             0, 0, 0,
             192, 160,
             7 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SKILL_TP_END] = 1;

    allParticleTextures[PARTICLE_KQ_SKILL_EXPLOSION] =
            {PARTICLE_KQ_SKILL_EXPLOSION, false,
             window->loadTexture("res/gfx/particle/KQSkillExplosion.png"),
             0, 0, 0,
             192, 160,
             14 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SKILL_EXPLOSION] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_AOE] =
            {PARTICLE_KQ_BURST_AOE, false,
             window->loadTexture("res/gfx/particle/KQBurstAOE.png"),
             0, 0, 0,
             200, 200,
             7 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_AOE] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_AOE_WAVE] =
            {PARTICLE_KQ_BURST_AOE_WAVE, false,
             window->loadTexture("res/gfx/particle/KQBurstAOEWave.png"),
             0, 0, 0,
             200, 200,
             12 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_AOE_WAVE] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_VANISH] =
            {PARTICLE_KQ_BURST_VANISH, false,
             window->loadTexture("res/gfx/particle/KQBurstVanish.png"),
             0, 0, 0,
             96, 96,
             6 * 96, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_VANISH] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_CLONE] =
            {PARTICLE_KQ_BURST_CLONE, false,
             window->loadTexture("res/gfx/particle/KQBurstClone.png"),
             0, 0, 0,
             208, 128,
             23 * 208, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_CLONE] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_CLONE_SLASH] =
            {PARTICLE_KQ_BURST_CLONE_SLASH, false,
             window->loadTexture("res/gfx/particle/KQBurstCloneSlash.png"),
             0, 0, 0,
             224, 64,
             7 * 224, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_CLONE_SLASH] = 1;

    allParticleTextures[PARTICLE_KQ_BURST_SLASH] =
            {PARTICLE_KQ_BURST_SLASH, false,
             window->loadTexture("res/gfx/particle/KQBurstSlash.png"),
             0, 0, 0,
             448, 32,
             7 * 448, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_SLASH] = KQ_BURST_NUMBER_OF_SLASH;

    allParticleTextures[PARTICLE_KQ_BURST_CLONE_VANISH] =
            {PARTICLE_KQ_BURST_CLONE_VANISH, false,
             window->loadTexture("res/gfx/particle/KQBurstCloneVanish.png"),
             0, 0, 0,
             96, 96,
             4 * 96, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_CLONE_VANISH] = KQ_BURST_NUMBER_OF_CLONE;

    allParticleTextures[PARTICLE_KQ_BURST_FINAL_SLASH] =
            {PARTICLE_KQ_BURST_FINAL_SLASH, false,
             window->loadTexture("res/gfx/particle/KQBurstFinalSlash.png"),
             0, 0, 0,
             800, 320,
             10 * 800, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_BURST_FINAL_SLASH] = 1;

    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        activeParticles[spriteCode] = (Particle **)
                calloc(activeParticleMaxes[spriteCode], sizeof(Particle *));
        counts[spriteCode] = 0;
    }
}

Particle *Particle::push(int spriteCode, int xShift, int yShift, int frameDuration,
                         float wMultiplier, float hMultiplier, Entity *entity_) {
    auto *particle = new Particle(spriteCode, xShift, yShift, frameDuration,
                                  wMultiplier, hMultiplier, entity_);
    pushFast(particle);
    return particle;
}

void Particle::pushFast(Particle *particle) {
    int spriteCode = particle->spriteArray[0].code;
    int count = counts[spriteCode];
    int i;
    if (count < activeParticleMaxes[spriteCode]) {
        i = count;
    } else { // Replace existing Particle (last one)
        i = count - 1;
        remove(spriteCode, i);
    }
    activeParticles[spriteCode][i] = particle;
    counts[spriteCode]++;
}

void Particle::remove(int spriteCode, int i) {
    if (i < 0 || i >= activeParticleMaxes[spriteCode]) {
        printf("WARNING, invalid index for particle remove!\n");
        return;
    }
    Particle *particle = activeParticles[spriteCode][i];
    if (particle == nullptr) return;

    int lastIndex = counts[spriteCode] - 1;
    particle->setRGBAMod(255, 255, 255, 255);
    delete particle;
    counts[spriteCode]--;
    activeParticles[spriteCode][i] = activeParticles[spriteCode][lastIndex];
    activeParticles[spriteCode][lastIndex] = nullptr;
}

void Particle::animateAll(int dt) {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            if (currParticle == nullptr) printf("Bizarre!\n");

            int lastXShift = currParticle->getXShift();
            int lastYShift = currParticle->getYShift();
            int lastXShiftR = currParticle->getXShiftR();
            currParticle->animate(dt);
            currParticle->setXYShift(lastXShift, lastYShift, lastXShiftR);

            if (currParticle->fadeParams.baseAlpha != -1) {
                Uint8 alpha;
                SDL_GetTextureAlphaMod(currParticle->texture, &alpha);
                alpha -= (int) ((float) dt * currParticle->fadeParams.speed *
                                ((float) currParticle->fadeParams.baseAlpha / 255.0f));
                if (alpha < 20) {
                    remove(spriteCode, i);
                    continue;
                }
                currParticle->setRGBAMod(255, 255, 255, alpha);
                continue;
            }

            if (currParticle->isFinished()) {
                if (currParticle->stopOnLastFrame) {
                    currParticle->setSpriteAnimated(0, true);
                    Sprite *sprite = &currParticle->spriteArray[0];
                    sprite->frameX = sprite->maxWidth - sprite->width;
                    sprite->frameDuration = INT32_MAX;
                } else if (currParticle->nextParticle != nullptr) {
                    Particle *nextParticle = currParticle->nextParticle;
                    if (nextParticle != currParticle) {
                        remove(spriteCode, i);
                        pushFast(nextParticle);
                    } else {
                        currParticle->setSpriteAnimated(0, true);
                        currParticle->reset(0);
                    }
                } else {
                    remove(spriteCode, i);
                }
            }
        }
    }
}

void Particle::renderAll(WindowRenderer *window, Entity *background) {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            if (currParticle == nullptr) printf("Bizarre!\n");

            window->renderParticle(currParticle, background);
        }
    }
}

Particle *Particle::getParticle(int spriteCode, int i) {
    return (activeParticles[spriteCode][i]);
}

bool Particle::isActive(int spriteCode, int i) {
    return (counts[spriteCode] > i);
}

void Particle::cleanUp() {
    for (int i = 0; i < PARTICLE_ENUM_N; i++) {
        SDL_DestroyTexture(allParticleTextures[i].texture);
    }
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            delete activeParticles[spriteCode][i];
        }
    }
}

void Particle::getToEntityCenterXY(Particle *centerParticle, int *pX, int *pY,
                                   int *pXShift, int *pYShift, int *pXShiftR) {
    float realW = (float) frame.w * renderWMultiplier * entity->getRenderWMultiplier();
    float realH = (float) frame.h * renderHMultiplier * entity->getRenderHMultiplier();

    SDL_Rect rect;
    int vX, vY;
    Entity *centerEntity;
    if (centerParticle == nullptr) {
        rect = entity->getCollisionRect();
        vX = entity->getX();
        vY = entity->getY();
        centerEntity = entity;
    } else {
        rect = {0, 0,
                (int) ((float) centerParticle->frame.w *
                       centerParticle->renderWMultiplier *
                       centerParticle->entity->getRenderWMultiplier()),
                (int) ((float) centerParticle->frame.h *
                       centerParticle->renderHMultiplier *
                       centerParticle->entity->getRenderHMultiplier())};
        vX = centerParticle->x;
        vY = centerParticle->y;
        centerEntity = centerParticle;
    }

    *pX = vX + rect.x +
          (int) ((float) rect.w / 2.0f - realW / 2.0f);
    *pY = vY + rect.y +
          (int) ((float) rect.h / 2.0f - realH / 2.0f);

    if (pXShift != nullptr)
        *pXShift = (int) ((float) xShift * centerEntity->getRenderWMultiplier());
    if (pYShift != nullptr)
        *pYShift = (int) ((float) yShift * centerEntity->getRenderHMultiplier());
    if (pXShiftR != nullptr)
        *pXShiftR = (int) ((float) -xShift * centerEntity->getRenderWMultiplier());
}

void Particle::moveToEntityCenter(Particle *centerParticle) {
    if (centerParticle == nullptr) facingEast = entity->isFacingEast();
    else facingEast = centerParticle->facingEast;
    getToEntityCenterXY(centerParticle, &x, &y,
                        &xShift, &yShift, &xShiftR);
}

bool Particle::isFinished() {
    return (!spriteArray[0].animated);
//    Sprite *currSprite = &spriteArray[0];
//    while (currSprite != nullptr) {
//        if (currSprite->animated) return false;
//        currSprite = currSprite->next;
//    }
//    return true;
}

void Particle::fadeAway(float speed) {
    Uint8 alpha;
    SDL_GetTextureAlphaMod(texture, &alpha);
    fadeParams.baseAlpha = alpha;
    fadeParams.speed = speed;
}
