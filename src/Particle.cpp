//
// Created by pyrowildx on 06/06/23.
//

#include "Particle.hpp"
#include "Keqing.hpp"

Sprite Particle::allParticleTextures[PARTICLE_END_ENUM];

int Particle::activeParticleMaxes[PARTICLE_END_ENUM];
Particle **Particle::activeParticles[PARTICLE_END_ENUM];
int Particle::counts[PARTICLE_END_ENUM];

Particle::Particle(int spriteCode, int xShift, int yShift, int frameDuration,
                   float wMultiplier, float hMultiplier, Entity *entity)
        : AnimatedEntity(false, 1), fadeParams({-1, 1.0f}) {
    spriteArray[0] = allParticleTextures[spriteCode];
    Sprite *sprite = &spriteArray[0];
    sprite->animated = true;
    sprite->xShift = xShift;
    sprite->yShift = yShift;
    sprite->xShiftR = -xShift;
    sprite->frameDuration = frameDuration;
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    this->entity = entity;
    entityDependant = true;
    stopOnLastFrame = false;
    nextParticle = nullptr;
}

void Particle::initParticle(WindowRenderer *window) {
    allParticleTextures[PARTICLE_KQ_NATTACK_4] =
            {PARTICLE_KQ_NATTACK_4, false,
             window->loadTexture("res/particles/kq_nattack_4.png"),
             0, 0, 0,
             80, 32,
             6 * 80, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_NATTACK_4] = 1;

    allParticleTextures[PARTICLE_KQ_AIR_NATTACK] =
            {PARTICLE_KQ_AIR_NATTACK, false,
             window->loadTexture("res/particles/kq_air_nattack.png"),
             0, 0, 0,
             32, 80,
             4 * 32, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_AIR_NATTACK] = 1;

    allParticleTextures[PARTICLE_KQ_AIR_NATTACK_GROUND] =
            {PARTICLE_KQ_AIR_NATTACK_GROUND, false,
             window->loadTexture("res/particles/kq_air_nattack_ground.png"),
             0, 0, 0,
             192, 96,
             6 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_AIR_NATTACK_GROUND] = 1;

    allParticleTextures[PARTICLE_KQ_SR_SPAWN] =
            {PARTICLE_KQ_SR_SPAWN, false,
             window->loadTexture("res/particles/kq_sr_spawn.png"),
             0, 0, 0,
             96, 96,
             8 * 96, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SR_SPAWN] = 1;

    allParticleTextures[PARTICLE_KQ_SR_IDLE] =
            {PARTICLE_KQ_SR_IDLE, false,
             window->loadTexture("res/particles/kq_sr_idle.png"),
             0, 0, 0,
             32, 32,
             4 * 32, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SR_IDLE] = 1;

    allParticleTextures[PARTICLE_KQ_SR_TP_START] =
            {PARTICLE_KQ_SR_TP_START, false,
             window->loadTexture("res/particles/kq_sr_tp_start.png"),
             0, 0, 0,
             192, 160,
             8 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SR_TP_START] = 1;

    allParticleTextures[PARTICLE_KQ_SR_TP_END] =
            {PARTICLE_KQ_SR_TP_END, false,
             window->loadTexture("res/particles/kq_sr_tp_end.png"),
             0, 0, 0,
             192, 160,
             7 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SR_TP_END] = 1;

    allParticleTextures[PARTICLE_KQ_SR_EXPLOSION] =
            {PARTICLE_KQ_SR_EXPLOSION, false,
             window->loadTexture("res/particles/kq_sr_explosion.png"),
             0, 0, 0,
             192, 160,
             14 * 192, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SR_EXPLOSION] = 1;

    allParticleTextures[PARTICLE_KQ_SS_AOE] =
            {PARTICLE_KQ_SS_AOE, false,
             window->loadTexture("res/particles/kq_ss_aoe.png"),
             0, 0, 0,
             200, 200,
             7 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_AOE] = 1;

    allParticleTextures[PARTICLE_KQ_SS_AOE_WAVE] =
            {PARTICLE_KQ_SS_AOE_WAVE, false,
             window->loadTexture("res/particles/kq_ss_aoe_wave.png"),
             0, 0, 0,
             200, 200,
             12 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_AOE_WAVE] = 1;

    allParticleTextures[PARTICLE_KQ_SS_VANISH] =
            {PARTICLE_KQ_SS_VANISH, false,
             window->loadTexture("res/particles/kq_ss_vanish.png"),
             0, 0, 0,
             96, 96,
             6 * 96, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_VANISH] = 1;

    allParticleTextures[PARTICLE_KQ_SS_CLONE] =
            {PARTICLE_KQ_SS_CLONE, false,
             window->loadTexture("res/particles/kq_ss_clone.png"),
             0, 0, 0,
             208, 128,
             23 * 208, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_CLONE] = 1;

    allParticleTextures[PARTICLE_KQ_SS_CLONE_SLASH] =
            {PARTICLE_KQ_SS_CLONE_SLASH, false,
             window->loadTexture("res/particles/kq_ss_clone_slash.png"),
             0, 0, 0,
             224, 64,
             7 * 224, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_CLONE_SLASH] = 1;

    allParticleTextures[PARTICLE_KQ_SS_SLASH] =
            {PARTICLE_KQ_SS_SLASH, false,
             window->loadTexture("res/particles/kq_ss_slash.png"),
             0, 0, 0,
             448, 32,
             7 * 448, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_SLASH] = KQ_SS_NUMBER_OF_SLASH;

    allParticleTextures[PARTICLE_KQ_SS_FINAL_SLASH] =
            {PARTICLE_KQ_SS_FINAL_SLASH, false,
             window->loadTexture("res/particles/kq_ss_final_slash.png"),
             0, 0, 0,
             800, 320,
             10 * 800, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_FINAL_SLASH] = 1;

    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
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
    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            if (currParticle == nullptr) printf("Bizarre!\n");

            currParticle->animate(dt);

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
                    sprite->currentFrameX = sprite->maxWidth - sprite->width;
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
    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
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
    for (int i = 0; i < PARTICLE_END_ENUM; i++) {
        SDL_DestroyTexture(allParticleTextures[i].texture);
    }
    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            delete activeParticles[spriteCode][i];
        }
    }
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
