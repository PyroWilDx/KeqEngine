//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AnimatedEntity.hpp"

#define MAX_ACTIVE_PARTICLES 64

enum {
    PARTICLE_KQ_ATTACK_5,
    PARTICLE_END_ENUM
};

static const char *imgPaths[PARTICLE_END_ENUM] = {
        "res/particles/kq_attack_5.png",
};

class Particle : public AnimatedEntity {

public:
    static void initParticle(WindowRenderer *window);

    static void push(int code, int xShift, int yShift,
                     float wMultiplier, float hMultiplier, Entity *entity_);

    static void remove(int i);

    static void cleanUp();

    static inline Particle **getActiveParticles() { return activeParticles; }

    static inline int getCount() { return count; }

    inline Entity *getEntity() { return entity; }

    inline bool isFinished() { return !spriteArray[0].animated; }

private:
    explicit Particle(int code, int xShift, int yShift,
                      float wMultiplier, float hMultiplier, Entity *entity_);

    static SpriteTexture allParticleTextures[PARTICLE_END_ENUM];

    static Particle *activeParticles[MAX_ACTIVE_PARTICLES];
    static int count;

    Entity *entity;

};

#endif
