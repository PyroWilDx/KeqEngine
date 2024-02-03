//
// Created by pyrowildx on 06/06/23.
//

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "AbstractEntity/AnimatedEntity.hpp"

enum {
    PARTICLE_KQ_NATK_4 = 0,
    PARTICLE_KQ_CATK,
    PARTICLE_KQ_UP_CATK,
    PARTICLE_KQ_CROUCH_CATK,
    PARTICLE_KQ_DASH_VANISH,
    PARTICLE_KQ_AIR_PLUNGE,
    PARTICLE_KQ_AIR_PLUNGE_GROUND,
    PARTICLE_KQ_ELECTRO_AURA,
    PARTICLE_KQ_SKILL_BLINK_0,
    PARTICLE_KQ_SKILL_BLINK_1,
    PARTICLE_KQ_SKILL_FLIP_SLASH,
    PARTICLE_KQ_SKILL_CLONE_FADE,
    PARTICLE_KQ_SKILL_CLONE_SPAWN,
    PARTICLE_KQ_SKILL_CLONE_IDLE,
    PARTICLE_KQ_SKILL_CLONE_DESPAWN,
    PARTICLE_KQ_SKILL_CURSOR_SPAWN,
    PARTICLE_KQ_SKILL_CURSOR,
    PARTICLE_KQ_SKILL_SHOOT,
    PARTICLE_KQ_SKILL_PROJ,
    PARTICLE_KQ_SKILL_PROJ_AFT_FX,
    PARTICLE_KQ_SKILL_SPAWN,
    PARTICLE_KQ_SKILL_IDLE,
    PARTICLE_KQ_SKILL_DESPAWN,
    PARTICLE_KQ_SKILL_TP_START,
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
    PARTICLE_DMG_PHYSICAL_0,
    PARTICLE_DMG_PHYSICAL_1,
    PARTICLE_DMG_PHYSICAL_2,
    PARTICLE_DMG_PHYSICAL_MINI,
    PARTICLE_DMG_ELECTRO_0,
    PARTICLE_DMG_ELECTRO_1,
    PARTICLE_DMG_ELECTRO_2,
    PARTICLE_DMG_ELECTRO_MINI,
    PARTICLE_EXPLOSION,
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

#define HUD_SKILL1_CIRCLE_RGBA 10, 254, 10
#define HUD_SKILL2_CIRCLE_RGBA 238, 10, 238
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

    static void initParticle();

    static Particle *pushParticle(int spriteCode, int frameLength,
                                  double wMultiplier = 1., double hMultiplier = 1.);

    static void pushFast(Particle *particle);

    static void removeParticle(int spriteCode, int i = 0);

    static void animateAll();

    static void renderAll();

    static void renderAllDebug(SDL_Renderer *gRenderer);

    static Particle *getParticle(int spriteCode, int i = 0);

    static bool isActive(int spriteCode, int i = 0);

    static void hardResetOnRender(int spriteCode);

    static void removeAllParticles();

    static void cleanUp();

    void getRealSize(double *pW, double *pH) override;

    bool shouldTranslate() override;

    void animateSprite() override;

    void setEntity(Entity *newEntity, bool sizeDependant = true,
                   bool takeFaceEast = true);

    void setParticleEntity(Particle *srcParticle, bool sizeDependant = true,
                           bool takeFaceEast = true);

    void xyShift(double xShift, double yShift);

    bool isFinished();

    void fadeAway(double speed = 1);

    void renderSelf(SDL_Renderer *gRenderer) override;

    void removeSelf();

    Particle *cloneSelf(int onRenderParamsSize = 0);

    inline void setEntitySizeDependant(bool entitySizeDependant_) {
        entitySizeDependant = entitySizeDependant_;
    }

    inline void setOnRender(void (*onRender_)(Particle *)) { onRender = onRender_; }

    inline void setOnRender(void (*onRender_)(Particle *), void *fParams) {
        onRender = onRender_;
        onRenderParams = fParams;
    }

    inline void setOnRemove(void (*onRemove_)(Particle *)) { onRemove = onRemove_; }

    [[nodiscard]] inline int getCode() const { return particleCode; }

    [[nodiscard]] inline auto getOnRender() const { return onRender; }

    [[nodiscard]] inline void *getOnRenderParams() const { return onRenderParams; }

private:
    static Particle *baseParticle;

    static int particleMaxActives[PARTICLE_ENUM_N];
    static Particle **activeParticles[PARTICLE_ENUM_N];
    static int activeCounts[PARTICLE_ENUM_N];

    int particleCode;
    Entity *entity;
    bool entitySizeDependant;
    double entityLastX, entityLastY;
    FadeAwayParams fadeParams;

    void (*onRender)(Particle *);

    void *onRenderParams;

    void (*onRemove)(Particle *);
};

#endif
