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
    PARTICLE_KQ_SS_AOE_WAVES,
    PARTICLE_KQ_SS_CLONES,
    PARTICLE_KQ_SS_CLONE_SLASH,
    PARTICLE_KQ_SS_SLASH,
    PARTICLE_KQ_SS_FINAL_SLASH,
    PARTICLE_END_ENUM
};

class Particle : public AnimatedEntity {

public:
    static void initParticle(WindowRenderer *window);

    static Particle *push(int spriteCode, int xShift, int yShift, int xShiftR,
                          int frameDuration, float wMultiplier, float hMultiplier,
                          Entity *entity);

    static void remove(int spriteCode, int i);

    static void animateAll(int dt);

    static void renderAll(WindowRenderer *window, Entity *background);

    static bool isActive(int spriteCode, int i);

//    void removeSelf();

//    static void removeParticleByCode(int spriteCode);

    static void cleanUp();

    inline Entity *getEntity() { return entity; }

    inline bool isFinished() { return !spriteArray[0].animated; }

    static inline int getCount() {
        int total = 0;
        for (int spriteCode = 0; spriteCode < PARTICLE_END_ENUM; spriteCode++) {
            total += counts[spriteCode];
        }
        return total;
    }

private:
    explicit Particle(int spriteCode, int xShift, int yShift, int xShiftR, int frameDuration,
                      float wMultiplier, float hMultiplier, Entity *entity);

    static Sprite allParticleTextures[PARTICLE_END_ENUM];

    static int activeParticleMaxes[PARTICLE_END_ENUM];
    static Particle **activeParticles[PARTICLE_END_ENUM];
    static int counts[PARTICLE_END_ENUM];

    Entity *entity;
//    int index;
};

#endif
