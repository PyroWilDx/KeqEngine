//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AnimatedEntity.hpp"

#define MAX_ACTIVE_PARTICLES 64

enum {
    PARTICLE_KQ_NATTACK_4 = 0,
    PARTICLE_KQ_AIR_NATTACK,
    PARTICLE_KQ_SS_AOE,
    PARTICLE_KQ_SS_AOE_WAVES,
    PARTICLE_KQ_SS_CLONES,
    PARTICLE_KQ_SS_SLASHES,
    PARTICLE_KQ_SS_FINAL_SLASH,
    PARTICLE_END_ENUM
};

class Particle : public AnimatedEntity {

public:
    static void initParticle(WindowRenderer *window);

    static void push(int spriteCode, int xShift, int yShift, int xShiftR, int frameDuration,
                     float wMultiplier, float hMultiplier, Entity *entity);

    static void remove(int i);

    static void cleanUp();

    static inline Particle **getActiveParticles() { return activeParticles; }

    static inline int getCount() { return count; }

    inline Entity *getEntity() { return entity; }

    inline bool isFinished() { return !spriteArray[0].animated; }

private:
    explicit Particle(int spriteCode, int xShift, int yShift, int xShiftR, int frameDuration,
                      float wMultiplier, float hMultiplier, Entity *entity);

    static Sprite allParticleTextures[PARTICLE_END_ENUM];

    static Particle *activeParticles[MAX_ACTIVE_PARTICLES];
    static int count;

    Entity *entity;

};

#endif
