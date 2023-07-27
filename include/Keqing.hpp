//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "BaseEntity/LivingEntity.hpp"

class Particle;

enum {
    KQ_IDLE = 0,
    KQ_RUN_END,
    KQ_JUMP_END,
    KQ_WALK,
    KQ_WALK_TURN,
    KQ_RUN_START,
    KQ_RUN,
    KQ_RUN_TURN,
    KQ_CROUCH,
    KQ_JUMP_START,
    KQ_JUMP,
    KQ_NATK,
    KQ_CATK,
    KQ_UP_NATK,
    KQ_UP_CATK,
    KQ_CROUCH_NATK,
    KQ_CROUCH_CATK,
    KQ_DASH,
    KQ_RUN_NATK,
    KQ_SKILL_FLIP,
    KQ_SKILL_FLIP_LAND,
    KQ_SKILL_CLONE,
    KQ_SKILL,
    KQ_SKILL_AIMING,
    KQ_SKILL_SLASH,
    KQ_BURST,
    KQ_AIR_DOUBLE_JUMP,
    KQ_AIR_NATK,
    KQ_AIR_UP_NATK,
    KQ_AIR_DASH,
    KQ_AIR_SKILL_CLONE,
    KQ_AIR_SKILL,
    KQ_AIR_SKILL_AIMING,
    KQ_AIR_SKILL_SLASH,
    KQ_AIR_PLUNGE,
    KQ_HURT,
    KQ_ENUM_N
};

#define KQ_BASE_COLOR 2861694463

#define KQ_WIDTH_MULTIPLIER 1.0
#define KQ_HEIGHT_MULTIPLIER 1.0

#define KQ_WALK_VELOCITY 0.4
#define KQ_RUN_VELOCITY 0.6
#define KQ_AIR_WALK_VELOCITY 0.38
#define KQ_DASH_VELOCITY 0.8

#define KQ_JUMP_BASE_VELOCITY 1.0
#define KQ_AIR_DOUBLE_JUMP_BASE_VELOCITY 0.8
#define KQ_AIR_DASH_VELOCITY 0.8

#define KQ_SKILL_FLIP_BASE_Y_VELOCITY 0.62
#define KQ_SKILL_FLIP_BASE_X_VELOCITY 0.44

#define KQ_SKILL_CLONE_X_VELOCITY 0.6
#define KQ_SKILL_CLONE_DURATION 8000

#define KQ_SKILL_AIR_AIMING_FLOAT_FRAME_N 120
#define KQ_SKILL_AIR_AIMING_FLOAT_VELOCITY 0.024
#define KQ_LIGHTNING_STELITTO_VELOCITY 2.4
#define KQ_SKILL_TP_MAX_DISTANCE 360
#define KQ_SKILL_COOLDOWN 7000
#define KQ_LIGHTNING_STILETTO_DURATION 6000

#define KQ_BURST_NUMBER_OF_CLONE_SLASH 6
#define KQ_BURST_NUMBER_OF_SLASH 8
#define KQ_BURST_NUMBER_OF_CLONE 5
#define KQ_BURST_COOLDOWN 10000

class Keqing : public LivingEntity {

public:
    void setSoundSheetStartPath();

    void setSoundSheet();

    static void initKeqing();

    static inline Keqing *getInstance() { return instance; }

    static void cleanUp();

    void reset();

    static void colorSprite(Uint32 rgba, Sprite *sprite);

    void colorCurrSprite(Uint32 rgba);

    void colorAllSprites(Uint32 rgba);

    void moveX() override;

    void setSpriteAnimated(bool animated, int spriteCode);

    bool shouldUpdateDirection();

    void updateDirection();

    bool canMoveLR();

    void moveLR();

    void airAnimate();

    void idle();

    void walk();

    void run();

    void crouch();

    void jump();

    void NAtk();

    static void checkESkillOnCAtk();

    void CAtk();

    void upNAtk();

    void upCAtk();

    void crouchNAtk();

    void crouchCAtk();

    void dash();

    void runNAtk();

    static Particle *pushParticleOnSkillBlink(Entity *centerEntity);

    static void pushElectroAura(Entity *srcEntity, Particle *srcParticle,
                                double renderWM = 1.0, double renderHM = 1.0,
                                int frameLength = 100);

    void ASkillFlip();

    void ASkillCloneGeneral();

    void ASkillClone();

    static void updateSkillHudIcon();

    static void createSkillSpawnParticle();

    static void createLightningStelitto();

    static void createSkillProjParticle();

    void getESkillSpriteCodes(int *pSkill, int *pSkillAiming, int *pSkillSlash);

    void ESkillGeneral();

    void ESkill();

    void ESkillAimingGeneral();

    void ESkillAiming();

    void ESkillSlashGeneral();

    void ESkillSlash();

    void RBurst();

    void airDoubleJump();

    void airNAtk();

    void airUpNAtk();

    void airDash();

    void airASkillClone();

    void airESkill();

    void airESkillAiming();

    void airESkillSlash();

    void airPlunge();

    void onGameFrame() override;

    void hurt() override;

    void setFacingEast(bool facingEast_) override;

    bool canDoAction(int spriteCode);

    static void preAction(int spriteCode, void *fParams);

    void updateActionFromKey();

    void updateAction() override;

    void kqLock(bool shouldLock);

    inline void setBurstCloneSlashCount(int burstCloneSlashCount_) { RBurstCloneSlashCount = burstCloneSlashCount_; }

    [[nodiscard]] inline double getESkillX() const { return ESkillX; }

    [[nodiscard]] inline double getESkillY() const { return ESkillY; }

    [[nodiscard]] inline int getESkillUseTime() const { return ESkillUseTime; }

    [[nodiscard]] inline int getRBurstCloneSlashCount() const { return RBurstCloneSlashCount; }

    [[nodiscard]] inline int getAirPlungeLoopSoundChannel() const { return airPlungeLoopSoundChannel; }

    [[nodiscard]] inline int getIsLocked() const { return isLocked; }

private:
    Keqing();

    static Keqing *instance;

    int jumpPressTime;
    int ASkillFlipPressTime;
    double ASkillCloneCenterX, ASkillCloneCenterY;
    int ESkillPausedSpriteCode;
    int ESkillCursorSoundChannel;
    double ESkillX, ESkillY;
    int ESkillUseTime;
    bool isESkillParticleInAir;
    int RBurstCloneSlashCount;
    bool airDoubleJumped;
    bool airDashed;
    bool airASkillCloned;
    int airESkillFloatCpt;
    int airPlungeLoopSoundChannel;
    bool isLocked;

};

#endif
