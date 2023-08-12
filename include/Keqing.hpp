//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "BaseEntity/LivingEntity.hpp"

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
    KQ_SKILL,
    KQ_SKILL_AIMING,
    KQ_SKILL_SLASH,
    KQ_BURST,
    KQ_AIR_DOUBLE_JUMP,
    KQ_AIR_NATK,
    KQ_AIR_UP_NATK,
    KQ_AIR_SKILL_SLASH,
    KQ_AIR_DASH,
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

#define LIGHTNING_STELITTO_VELOCITY 2.4
#define IDLE_PARTICLE_GAP 12
#define SKILL_TP_DISTANCE 360

#define KQ_BURST_NUMBER_OF_CLONE_SLASH 6
#define KQ_BURST_NUMBER_OF_SLASH 8
#define KQ_BURST_NUMBER_OF_CLONE 5

#define KQ_SKILL_COOLDOWN 7000
#define KQ_LIGHTNING_STILETTO_DURATION 6000
//#define KQ_BURST_COOLDOWN 10000
#define KQ_SKILL_CIRCLE_RGBA 10, 255, 10, 255

class Keqing : public LivingEntity {

public:
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

    void CAtk();

    void upNAtk();

    void upCAtk();

    void crouchNAtk();

    void crouchCAtk();

    void dash();

    void createLightningStelitto(int mouseX = -1, int mouseY = -1);

    void ESkill();

    void ESkillAiming();

    void doSkillSlashFrame0();

    void moveToLStiletto();

    void doSkillTP();

    void ESkillSlash();

    void RBurst();

    void airDoubleJump();

    void airNAtk();

    void airUpNAtk();

    void airESkillSlash();

    void airDash();

    void airPlunge();

    void onGameFrame() override;

    void hurt() override;

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    static void preAction(int spriteCode, void *fParams);

    void updateActionFromKey();

    void updateAction() override;

    void lock();

    void unlock();

    inline void setBurstCloneSlashCount(int burstCloneSlashCount_) { burstCloneSlashCount = burstCloneSlashCount_; }

    [[nodiscard]] inline int getSkillUseTime() const { return skillUseTime; }

    [[nodiscard]] inline int getBurstCloneSlashCount() const { return burstCloneSlashCount; }

    [[nodiscard]] inline int isLocked() const { return locked; }

private:
    Keqing();

    static Keqing *instance;

    int burstCloneSlashCount;
    int skillUseTime;
    int jumpPressTime;
    bool doubleJumped;
    bool airDashed;
    int skillAimingSoundChannel;
    int airPlungeLoopSoundChannel;
    bool locked;

};

#endif
