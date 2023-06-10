//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AnimatedEntity.hpp"

enum {
    PARTICLE_KQ_NATTACK_4 = 0,
    PARTICLE_KQ_AIR_NATTACK,
    PARTICLE_KQ_AIR_NATTACK_GROUND,
    PARTICLE_KQ_SS_AOE,
    PARTICLE_KQ_SS_AOE_WAVE,
    PARTICLE_KQ_SS_VANISH,
    PARTICLE_KQ_SS_CLONE,
    PARTICLE_KQ_SS_CLONE_SLASH,
    PARTICLE_KQ_SS_SLASH,
    PARTICLE_KQ_SS_FINAL_SLASH,
    PARTICLE_END_ENUM
};

typedef struct FadeAwayParams {
    int baseAlpha;
    float speed;
} FadeAwayParams;

class Particle : public AnimatedEntity {

public:
    Particle(int spriteCode, int xShift, int yShift, int frameDuration,
             float wMultiplier, float hMultiplier, Entity *entity);

    static void initParticle(WindowRenderer *window);

    static Particle *push(int spriteCode, int xShift, int yShift, int frameDuration,
                          float wMultiplier, float hMultiplier, Entity *entity);

    static void pushFast(Particle *particle);

    static void remove(int spriteCode, int i);

    static void animateAll(int dt);

    static void renderAll(WindowRenderer *window, Entity *background);

    static Particle *getParticle(int spriteCode, int i);

    static bool isActive(int spriteCode, int i);

    static void cleanUp();

    static inline int getCount() {
        int total = 0;
        for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
            total += counts[spriteCode];
        }
        return total;
    }

    bool isFinished();

    void fadeAway(float speed = 1.0f);

    inline void setStopOnLastFrame(bool stop) { stopOnLastFrame = stop; }

    inline void setNextParticle(Particle *particle) { nextParticle = particle; }

    inline Entity *getEntity() { return entity; }

private:
    static Sprite allParticleTextures[PARTICLE_END_ENUM];

    static int activeParticleMaxes[PARTICLE_END_ENUM];
    static Particle **activeParticles[PARTICLE_END_ENUM];
    static int counts[PARTICLE_END_ENUM];

    Entity *entity;
    bool stopOnLastFrame;
    Particle *nextParticle;
    FadeAwayParams fadeParams;
};

#endif
