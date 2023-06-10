//
// Created by pyrowildx on 06/06/23.
//

#include "Particle.hpp"

Sprite Particle::allParticleTextures[PARTICLE_END_ENUM];

int Particle::activeParticleMaxes[PARTICLE_END_ENUM];
Particle **Particle::activeParticles[PARTICLE_END_ENUM];
int Particle::counts[PARTICLE_END_ENUM];

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
//    index = counts[spriteCode];
}

void Particle::initParticle(WindowRenderer *window) {
    allParticleTextures[PARTICLE_KQ_NATTACK_4] =
            {PARTICLE_KQ_NATTACK_4, false,
             window->loadTexture("res/particles/kq_attack_4.png"),
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

    allParticleTextures[PARTICLE_KQ_SS_AOE] =
            {PARTICLE_KQ_SS_AOE, false,
             window->loadTexture("res/particles/kq_ss_aoe.png"),
             0, 0, 0,
             200, 200,
             7 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_AOE] = 1;

    allParticleTextures[PARTICLE_KQ_SS_AOE_WAVES] =
            {PARTICLE_KQ_SS_AOE_WAVES, false,
             window->loadTexture("res/particles/kq_ss_aoe_waves.png"),
             0, 0, 0,
             200, 200,
             12 * 200, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_AOE_WAVES] = 1;

    allParticleTextures[PARTICLE_KQ_SS_CLONES] =
            {PARTICLE_KQ_SS_CLONES, false,
             window->loadTexture("res/particles/kq_ss_clones.png"),
             0, 0, 0,
             208, 128,
             23 * 208, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_CLONES] = 1;

    allParticleTextures[PARTICLE_KQ_SS_CLONE_SLASH] =
            {PARTICLE_KQ_SS_CLONE_SLASH, false,
             window->loadTexture("res/particles/kq_ss_clone_slash.png"),
             0, 0, 0,
             128, 128,
             5 * 128, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_CLONE_SLASH] = 1;

    allParticleTextures[PARTICLE_KQ_SS_SLASH] =
            {PARTICLE_KQ_SS_SLASH, false,
             window->loadTexture("res/particles/kq_ss_slash.png"),
             0, 0, 0,
             448, 32,
             7 * 448, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_SLASH] = 1;

    allParticleTextures[PARTICLE_KQ_SS_FINAL_SLASH] =
            {PARTICLE_KQ_SS_FINAL_SLASH, false,
             window->loadTexture("res/particles/kq_ss_final_slash.png"),
             0, 0, 0,
             800, 320,
             10 * 800, 0,
             0, 0, nullptr};
    activeParticleMaxes[PARTICLE_KQ_SS_FINAL_SLASH] = 1;

    for (int i = 0; i < PARTICLE_END_ENUM; i++) {
        activeParticles[i] = (Particle **)
                malloc(activeParticleMaxes[i] * sizeof(Particle *));
        counts[i] = 0;
    }
}

Particle *Particle::push(int spriteCode, int xShift, int yShift, int xShiftR,
                         int frameDuration, float wMultiplier, float hMultiplier,
                         Entity *entity_) {
    int count = counts[spriteCode];
    if (count < activeParticleMaxes[spriteCode]) {
        auto *particle = new Particle(spriteCode, xShift, yShift, xShiftR,
                                      frameDuration, wMultiplier, hMultiplier,
                                      entity_);
        activeParticles[spriteCode][count] = particle;
        counts[spriteCode]++;
        return particle;
    } else {
        printf("Maximum number of Particles Reached!\n");
    }
    return nullptr;
}

void Particle::remove(int spriteCode, int i) {
    if (i < 0 || i >= activeParticleMaxes[spriteCode]) {
        printf("WARNING, invalid index for particle remove!\n");
        return;
    }
    if (activeParticles[spriteCode][i] == nullptr) return;

    int lastIndex = counts[spriteCode] - 1;
    delete activeParticles[spriteCode][i];
    counts[spriteCode]--;
    activeParticles[spriteCode][i] = activeParticles[spriteCode][lastIndex];
    activeParticles[spriteCode][i] = nullptr;
}

void Particle::animateAll(int dt) {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            if (currParticle == nullptr) break;
            currParticle->animate(dt);
            if (currParticle->isFinished()) {
                remove(spriteCode, i);
            }
        }
    }
}

void Particle::renderAll(WindowRenderer *window, Entity *background) {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
        for (int i = 0; i < counts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            if (currParticle == nullptr) break;
            window->renderParticle(currParticle, background);
        }
    }
}

bool Particle::isActive(int spriteCode, int i) {
    return (counts[spriteCode] > i);
}

//void Particle::removeSelf() {
//    remove(index);
//}

//void Particle::removeParticleByCode(int spriteCode) {
//    for (int i = 0; i < MAX_ACTIVE_PARTICLE; i++) {
//        if (activeParticles[i] == nullptr) break;
//
//        if (activeParticles[i]->spriteArray->code == spriteCode) {
//            Particle::remove(i);
//        }
//    }
//}

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
