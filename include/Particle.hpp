//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AnimatedEntity.hpp"

enum {
    PARTICLE_KQ_NATK_4 = 0,
    PARTICLE_KQ_CATK,
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

typedef struct FadeAwayParams {
    int baseAlpha;
    double speed;
} FadeAwayParams;

class Particle : public AnimatedEntity {

public:
    Particle();

    Particle(int spriteCode, int frameLength, double wMultiplier, double hMultiplier);

    ~Particle() override = default;

    static void initParticle();

    static Particle *push(int spriteCode, int frameLength, double wMultiplier, double hMultiplier);

    static void pushFast(Particle *particle);

    static void remove(int spriteCode, int i);

    static void animateAll();

    static void renderAll();

    static Particle *getParticle(int spriteCode, int i);

    static bool isActive(int spriteCode, int i);

    static void cleanUp();

    void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) override;

    void getRealSize(double *pW, double *pH) override;

    bool shouldTranslate() override;

    void setEntity(Entity *newEntity);

    void getToEntityCenterXY(Entity *centerEntity, double *pX, double *pY);

    void moveToEntityCenter(Entity *centerEntity);

    void xyShift(double xShift, double yShift);

    bool isFinished();

    void fadeAway(double speed = 1);

    Particle *copy();

    inline void setOnRemove(void (*onRemove_)(Particle *)) { onRemove = onRemove_; }

    [[nodiscard]] inline int getCode() const { return code; }

//    inline void updateSprite(int spriteCode) override {};

private:
    static Particle *baseParticle;

    static int particleMaxActives[PARTICLE_ENUM_N];
    static Particle **activeParticles[PARTICLE_ENUM_N];
    static int activeCounts[PARTICLE_ENUM_N];

    int code;
    Entity *entity;
    double entityLastX, entityLastY;
    FadeAwayParams fadeParams;

    void (*onRemove)(Particle *);
};

#endif
