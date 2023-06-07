//
// Created by pyrowildx on 06/06/23.
//

#include "Particle.hpp"

SpriteTexture Particle::allParticleTextures[PARTICLE_END_ENUM];
Particle *Particle::activeParticles[MAX_ACTIVE_PARTICLES];
int Particle::count = 0;

Particle::Particle(int code, int xShift, int yShift, int xShiftR,
                   float wMultiplier, float hMultiplier, Entity *entity)
        : AnimatedEntity(false, 1) {
    spriteArray[0] = allParticleTextures[code];
    spriteArray[0].xShift = xShift;
    spriteArray[0].yShift = yShift;
    spriteArray[0].xShiftR = xShiftR;
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    this->entity = entity;
}

void Particle::initParticle(WindowRenderer *window) {
    allParticleTextures[PARTICLE_KQ_NATTACK_4] =
            {true, true,
             window->loadTexture(imgPaths[PARTICLE_KQ_NATTACK_4]),
             0, 0, 0,
             80, 32,
             6 * 80, 60,
             0, 0,
             nullptr};

    for (int i = 0; i < MAX_ACTIVE_PARTICLES; i++) {
        activeParticles[i] = nullptr;
    }
}

void Particle::push(int code, int xShift, int yShift, int xShiftR,
                    float wMultiplier, float hMultiplier, Entity *entity_) {
    if (count < MAX_ACTIVE_PARTICLES) {
        activeParticles[count] = new Particle(code, xShift, yShift, xShiftR,
                                              wMultiplier, hMultiplier, entity_);
        count++;
    } else {
        printf("Maximum number of Particles Reached!\n");
    }
}

void Particle::remove(int i) {
    delete activeParticles[i];
    activeParticles[i] = nullptr;
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
