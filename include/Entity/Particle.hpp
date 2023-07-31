//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "EntityBase/AnimatedEntity.hpp"

enum {
    PARTICLE_KQ_NATK_4 = 0,
    PARTICLE_KQ_CATK,
    PARTICLE_KQ_UP_CATK,
    PARTICLE_KQ_CROUCH_CATK,
    PARTICLE_KQ_AIR_PLUNGE,
    PARTICLE_KQ_AIR_PLUNGE_GROUND,
    PARTICLE_KQ_SKILL_SPAWN,
    PARTICLE_KQ_SKILL_IDLE,
    PARTICLE_KQ_SKILL_TP_END,
    PARTICLE_KQ_SKILL_EXPLOSION,
    PARTICLE_KQ_SKILL_AIMING_IDLE,
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
    PARTICLE_HUD_COLOR_PICKER_CURSOR,
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
    explicit Particle(bool isBaseParticle);

    Particle(int spriteCode, int frameLength, double wMultiplier, double hMultiplier);

    ~Particle() override = default;

    static void initParticle();

    static Particle *pushParticle(int spriteCode, int frameLength,
                                  double wMultiplier = 1.0, double hMultiplier = 1.0);

    static void pushFast(Particle *particle);

    static void removeParticle(int spriteCode, int i = 0);

    static void animateAll();

    static void renderAll();

    static Particle *getParticle(int spriteCode, int i = 0);

    static bool isActive(int spriteCode, int i = 0);

    static void removeAllParticles();

    static void cleanUp();

    void getRealSize(double *pW, double *pH) override;

    bool shouldTranslate() override;

    void animateSprite() override;

    void setEntity(Entity *newEntity);

    void xyShift(double xShift, double yShift);

    bool isFinished();

    void fadeAway(double speed = 1);

    Particle *copy();

    inline void setOnRender(void (*onRender_)(Particle *)) { onRender = onRender_; }

    inline void setOnRemove(void (*onRemove_)(Particle *)) { onRemove = onRemove_; }

    [[nodiscard]] inline int getCode() const { return particleCode; }

private:
    static Particle *baseParticle;

    static int particleMaxActives[PARTICLE_ENUM_N];
    static Particle **activeParticles[PARTICLE_ENUM_N];
    static int activeCounts[PARTICLE_ENUM_N];

    int particleCode;
    Entity *entity;
    double entityLastX, entityLastY;
    FadeAwayParams fadeParams;

    void (*onRender)(Particle *);

    void (*onRemove)(Particle *);
};

#endif
