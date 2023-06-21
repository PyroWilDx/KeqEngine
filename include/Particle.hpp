//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AnimatedEntity.hpp"

enum {
    PARTICLE_KQ_NATK_4 = 0,
    PARTICLE_KQ_AIR_NATK,
    PARTICLE_KQ_AIR_NATK_GROUND,
    PARTICLE_KQ_SKILL_SPAWN,
    PARTICLE_KQ_SKILL_IDLE,
    PARTICLE_KQ_SKILL_TP_END,
    PARTICLE_KQ_SKILL_EXPLOSION,
    PARTICLE_KQ_BURST_AOE,
    PARTICLE_KQ_BURST_AOE_WAVE,
    PARTICLE_KQ_BURST_VANISH,
    PARTICLE_KQ_BURST_CLONE,
    PARTICLE_KQ_BURST_CLONE_APPEAR,
    PARTICLE_KQ_BURST_CLONE_SLASH,
    PARTICLE_KQ_BURST_SLASH,
    PARTICLE_KQ_BURST_CLONE_VANISH,
    PARTICLE_KQ_BURST_FINAL_SLASH,
    PARTICLE_HUD_START,
    PARTICLE_HUD_SKILL_CIRCLE_BG,
    PARTICLE_HUD_BURST_CIRCLE_BG,
    PARTICLE_HUD_SKILL_CIRCLE,
    PARTICLE_HUD_BURST_CIRCLE,
    PARTICLE_HUD_SKILL_ICON_1,
    PARTICLE_HUD_SKILL_ICON_2,
    PARTICLE_HUD_BURST_ICON,
    PARTICLE_HUD_SKILL_BURST_TIMER,
    PARTICLE_ENUM_N
};

#define HUD_SB_TIMER_FRAME_N 25
#define HUD_SB_USED_ALPHA 64

#define MAX_NEXT_PARTICLE 4

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
        for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
            total += counts[spriteCode];
        }
        return total;
    }

    void getRealSize(float *pW, float *pH) override;

    void setFrameX(int x);

    void getToEntityCenterXY(Particle *centerParticle, float *pX, float *pY,
                             int *pXShift = nullptr, int *pYShift = nullptr,
                             int *pXShiftR = nullptr);

    void moveToEntityCenter(Particle *centerParticle);

    void addNextParticle(Particle *particle);

    bool isFinished();

    void fadeAway(float speed = 1.0f);

    Particle *copy();

    inline void setEntityDependant(bool dependant) { entityDependant = dependant; }

    inline void setStopOnLastFrame(bool stop) { stopOnLastFrame = stop; }

    inline void setOnRemove(void (*onRemove_)(Particle *)) { onRemove = onRemove_; }

    inline Entity *getEntity() { return entity; }

    inline bool isEntityDependant() { return entityDependant; }

    inline Particle *getNextParticle(int i) { return nextParticle[i]; }

private:
    static Sprite allParticleTextures[PARTICLE_ENUM_N];

    static int activeParticleMaxes[PARTICLE_ENUM_N];
    static Particle **activeParticles[PARTICLE_ENUM_N];
    static int counts[PARTICLE_ENUM_N];

    Entity *entity;
    bool entityDependant;
    bool stopOnLastFrame;
    Particle *nextParticle[MAX_NEXT_PARTICLE];
    FadeAwayParams fadeParams;

    void (*onRemove)(Particle *);
};

#endif
