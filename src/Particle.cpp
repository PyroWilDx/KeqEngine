//
// Created by pyrowildx on 06/06/23.
//

#include "Particle.hpp"

Sprite Particle::allParticleTextures[PARTICLE_END_ENUM];
Particle *Particle::activeParticles[MAX_ACTIVE_PARTICLES];
int Particle::count = 0;

Particle::Particle(int spriteCode, int xShift, int yShift, int xShiftR, int frameDuration,
                   float wMultiplier, float hMultiplier, Entity *entity)
        : AnimatedEntity(false, 1) {
    spriteArray[0] = allParticleTextures[spriteCode];
    Sprite *sprite = &spriteArray[0];
    sprite->animated = true;
    sprite->xShift = xShift;
    sprite->yShift = yShift;
    sprite->xShiftR = xShiftR;
    sprite->frameDuration = frameDuration;
    renderWMultiplier = wMultiplier * entity->getRenderWMultiplier();
    renderHMultiplier = hMultiplier * entity->getRenderHMultiplier();
    this->entity = entity;
}

void Particle::initParticle(WindowRenderer *window) {
    allParticleTextures[PARTICLE_KQ_NATTACK_4] =
            {PARTICLE_KQ_NATTACK_4, false,
             window->loadTexture("res/particles/kq_attack_4.png"),
             0, 0, 0,
             80, 32,
             6 * 80, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_AIR_NATTACK] =
            {PARTICLE_KQ_AIR_NATTACK, false,
             window->loadTexture("res/particles/kq_air_nattack.png"),
             0, 0, 0,
             32, 80,
             4 * 32, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_SS_AOE] =
            {PARTICLE_KQ_SS_AOE, false,
             window->loadTexture("res/particles/kq_ss_aoe.png"),
             0, 0, 0,
             200, 200,
             7 * 200, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_SS_AOE_WAVES] =
            {PARTICLE_KQ_SS_AOE_WAVES, false,
             window->loadTexture("res/particles/kq_ss_aoe_waves.png"),
             0, 0, 0,
             200, 200,
             12 * 200, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_SS_CLONES] =
            {PARTICLE_KQ_SS_CLONES, false,
             window->loadTexture("res/particles/kq_ss_clones.png"),
             0, 0, 0,
             208, 128,
             23 * 208, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_SS_SLASHES] =
            {PARTICLE_KQ_SS_SLASHES, false,
             window->loadTexture("res/particles/kq_ss_slashes.png"),
             0, 0, 0,
             448, 32,
             7 * 448, 0,
             0, 0, nullptr};

    allParticleTextures[PARTICLE_KQ_SS_FINAL_SLASH] =
            {PARTICLE_KQ_SS_FINAL_SLASH, false,
             window->loadTexture("res/particles/kq_ss_final_slash.png"),
             0, 0, 0,
             800, 320,
             10 * 800, 0,
             0, 0, nullptr};

    for (int i = 0; i < MAX_ACTIVE_PARTICLES; i++) {
        activeParticles[i] = nullptr;
    }
}

void Particle::push(int spriteCode, int xShift, int yShift, int xShiftR, int frameDuration,
                    float wMultiplier, float hMultiplier, Entity *entity_) {
    if (count < MAX_ACTIVE_PARTICLES) {
        activeParticles[count] = new Particle(spriteCode, xShift, yShift, xShiftR, frameDuration,
                                              wMultiplier, hMultiplier, entity_);
        count++;
    } else {
        printf("Maximum number of Particles Reached!\n");
    }
}

void Particle::remove(int i) {
    int lastIndex = count - 1;
    delete activeParticles[i];
    activeParticles[i] = activeParticles[lastIndex];
    activeParticles[lastIndex] = nullptr;
    count--;
}

void Particle::cleanUp() {
    for (int i = 0; i < PARTICLE_END_ENUM; i++) {
        SDL_DestroyTexture(allParticleTextures[i].texture);
    }
    for (int i = 0; i < MAX_ACTIVE_PARTICLES; i++) {
        delete activeParticles[i];
    }
}
