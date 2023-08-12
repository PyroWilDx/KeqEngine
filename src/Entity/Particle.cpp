//
// Created by pyrowildx on 06/06/23.
//

#include "Entity/Particle.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"

Particle *Particle::baseParticle = new Particle(true);

int Particle::particleMaxActives[PARTICLE_ENUM_N];
Particle **Particle::activeParticles[PARTICLE_ENUM_N];
int Particle::activeCounts[PARTICLE_ENUM_N];

Particle::Particle(bool isBaseParticle)
        : AnimatedEntity((isBaseParticle) ? PARTICLE_ENUM_N : 1),
          fadeParams({-1, 1}) {
    particleCode = -1;
    entity = nullptr;
    entityLastX = 0;
    entityLastY = 0;
    onRender = nullptr;
    onRenderParams = nullptr;
    onRemove = nullptr;
}

Particle::Particle(int spriteCode, int frameLength, double wMultiplier, double hMultiplier)
        : Particle(false) {
    this->particleCode = spriteCode;
    Sprite *baseSprite = baseParticle->getSprite(spriteCode);
    Sprite *currentSprite = getSprite();
    *currentSprite = *baseSprite;
    currentSprite->sCode = 0;
    AnimatedEntity::setSpriteAnimated(true);
    setSpriteFrameLengthFromTo(frameLength, 0, -1);
    if (baseSprite->sNext != nullptr) { // If != NULL, then repeat itself
        currentSprite->sNext = currentSprite;
    }
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    imgFrame = {0, 0, currentSprite->sFrameW, currentSprite->sFrameH};
    imgTexture = currentSprite->sTexture;
}

Particle::~Particle() {
    if (particleCode == -1) {
        baseParticle->imgTexture = (SDL_Texture *) 42; // Delete Texture (Clean Up)
    } else {
        imgTexture = nullptr; // Don't Delete Texture (Not the End)
    }
}

void Particle::initParticle() {
    baseParticle->initSprite(PARTICLE_KQ_NATK_4, "res/gfx/particle/KQNAtk4.png",
                             80, 32, 6);
    particleMaxActives[PARTICLE_KQ_NATK_4] = 1;

    baseParticle->initSprite(PARTICLE_KQ_CATK, "res/gfx/particle/KQCAtk.png",
                             160, 96, 6);
    particleMaxActives[PARTICLE_KQ_CATK] = 1;

    baseParticle->initSprite(PARTICLE_KQ_UP_CATK, "res/gfx/particle/KQUpCAtk.png",
                             96, 160, 6);
    particleMaxActives[PARTICLE_KQ_UP_CATK] = 1;

    baseParticle->initSprite(PARTICLE_KQ_CROUCH_CATK, "res/gfx/particle/KQCrouchCAtk.png",
                             224, 64, 6);
    particleMaxActives[PARTICLE_KQ_CROUCH_CATK] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_PLUNGE, "res/gfx/particle/KQAirNAtk.png",
                             32, 80, 4);
    baseParticle->setSpriteNext(PARTICLE_KQ_AIR_PLUNGE, PARTICLE_KQ_AIR_PLUNGE);
    particleMaxActives[PARTICLE_KQ_AIR_PLUNGE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_PLUNGE_GROUND, "res/gfx/particle/KQAirNAtkGround.png",
                             192, 96, 6);
    particleMaxActives[PARTICLE_KQ_AIR_PLUNGE_GROUND] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_PROJ, "res/gfx/particle/KQSkillProj.png",
                             64, 32, 1);
    particleMaxActives[PARTICLE_KQ_SKILL_PROJ] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_PROJ_AFT_FX, "res/gfx/particle/KQSkillProjAftFx.png",
                             64, 32, 6);
    particleMaxActives[PARTICLE_KQ_SKILL_PROJ_AFT_FX] = 64;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_SPAWN, "res/gfx/particle/KQSkillSpawn.png",
                             96, 96, 8);
    particleMaxActives[PARTICLE_KQ_SKILL_SPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_IDLE, "res/gfx/particle/KQSkillIdle.png",
                             32, 32, 4);
    baseParticle->setSpriteNext(PARTICLE_KQ_SKILL_IDLE, PARTICLE_KQ_SKILL_IDLE);
    particleMaxActives[PARTICLE_KQ_SKILL_IDLE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_TP_END, "res/gfx/particle/KQSkillTPEnd.png",
                             192, 160, 7);
    particleMaxActives[PARTICLE_KQ_SKILL_TP_END] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_EXPLOSION, "res/gfx/particle/KQSkillExplosion.png",
                             192, 160, 14);
    particleMaxActives[PARTICLE_KQ_SKILL_EXPLOSION] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_AIMING_IDLE, "res/gfx/particle/KQSkillAimingIdle.png",
                             48, 48, 8);
    baseParticle->setSpriteNext(PARTICLE_KQ_SKILL_AIMING_IDLE, PARTICLE_KQ_SKILL_AIMING_IDLE);
    particleMaxActives[PARTICLE_KQ_SKILL_AIMING_IDLE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_AOE, "res/gfx/particle/KQBurstAOE.png",
                             200, 200, 7);
    baseParticle->setSpriteNext(PARTICLE_KQ_BURST_AOE, PARTICLE_KQ_BURST_AOE);
    particleMaxActives[PARTICLE_KQ_BURST_AOE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_AOE_WAVE, "res/gfx/particle/KQBurstAOEWave.png",
                             200, 200, 12);
    baseParticle->setSpriteNext(PARTICLE_KQ_BURST_AOE_WAVE, PARTICLE_KQ_BURST_AOE_WAVE);
    particleMaxActives[PARTICLE_KQ_BURST_AOE_WAVE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_VANISH, "res/gfx/particle/KQBurstVanish.png",
                             96, 96, 6);
    particleMaxActives[PARTICLE_KQ_BURST_VANISH] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE, "res/gfx/particle/KQBurstClone.png",
                             208, 128, 23);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_APPEAR, "res/gfx/particle/KQBurstCloneAppear.png",
                             96, 96, 7);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_APPEAR] = KQ_BURST_NUMBER_OF_CLONE;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_SLASH, "res/gfx/particle/KQBurstCloneSlash.png",
                             224, 64, 7);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_SLASH] = 6;

    baseParticle->initSprite(PARTICLE_KQ_BURST_SLASH, "res/gfx/particle/KQBurstSlash.png",
                             448, 32, 7);
    particleMaxActives[PARTICLE_KQ_BURST_SLASH] = KQ_BURST_NUMBER_OF_SLASH;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_VANISH, "res/gfx/particle/KQBurstCloneVanish.png",
                             96, 96, 4);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_VANISH] = KQ_BURST_NUMBER_OF_CLONE;

    baseParticle->initSprite(PARTICLE_KQ_BURST_FINAL_SLASH, "res/gfx/particle/KQBurstFinalSlash.png",
                             800, 320, 10);
    particleMaxActives[PARTICLE_KQ_BURST_FINAL_SLASH] = 1;

    baseParticle->initSprite(PARTICLE_HUD_START, nullptr,
                             0, 0, 0);
    particleMaxActives[PARTICLE_HUD_START] = 0;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_CIRCLE_BG, "res/gfx/hud/SkillBurstCircleBG.png",
                             32, 32, 2);
    particleMaxActives[PARTICLE_HUD_SKILL_CIRCLE_BG] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_CIRCLE_BG, "res/gfx/hud/SkillBurstCircleBG.png",
                             32, 32, 2);
    baseParticle->goToFrame(1, PARTICLE_HUD_BURST_CIRCLE_BG);
    particleMaxActives[PARTICLE_HUD_BURST_CIRCLE_BG] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_CIRCLE, "res/gfx/hud/SkillBurstCircle.png",
                             32, 32, 2);
    particleMaxActives[PARTICLE_HUD_SKILL_CIRCLE] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_CIRCLE, "res/gfx/hud/SkillBurstCircle.png",
                             32, 32, 2);
    baseParticle->goToFrame(1, PARTICLE_HUD_BURST_CIRCLE);
    particleMaxActives[PARTICLE_HUD_BURST_CIRCLE] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_ICON_1, "res/gfx/hud/SkillIcon1.png",
                             21, 21, 1);
    particleMaxActives[PARTICLE_HUD_SKILL_ICON_1] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_ICON_2, "res/gfx/hud/SkillIcon2.png",
                             21, 21, 1);
    particleMaxActives[PARTICLE_HUD_SKILL_ICON_2] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_ICON, "res/gfx/hud/BurstIcon.png",
                             21, 21, 1);
    particleMaxActives[PARTICLE_HUD_BURST_ICON] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_BURST_TIMER, "res/gfx/hud/SkillBurstTimer.png",
                             32, 32, HUD_SB_TIMER_FRAME_N);
    particleMaxActives[PARTICLE_HUD_SKILL_BURST_TIMER] = 2;

    baseParticle->initSprite(PARTICLE_HUD_COLOR_PICKER_CURSOR, "res/gfx/hud/ColorPickerCursor.png",
                             16, 16, 2);
    particleMaxActives[PARTICLE_HUD_COLOR_PICKER_CURSOR] = 1;

    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        activeParticles[spriteCode] = (Particle **)
                calloc(particleMaxActives[spriteCode], sizeof(Particle *));
        activeCounts[spriteCode] = 0;
    }
}

Particle *Particle::pushParticle(int spriteCode, int frameLength,
                                 double wMultiplier, double hMultiplier) {
    auto *particle = new Particle(spriteCode, frameLength, wMultiplier, hMultiplier);
    pushFast(particle);
    return particle;
}

void Particle::pushFast(Particle *particle) {
    int spriteCode = particle->particleCode;
    int count = activeCounts[spriteCode];
    int i;
    if (count < particleMaxActives[spriteCode]) {
        i = count;
    } else { // Replace existing Particle (last one)
        i = count - 1;
        removeParticle(spriteCode, i);
    }
    activeParticles[spriteCode][i] = particle;
    activeCounts[spriteCode]++;
}

void Particle::removeParticle(int spriteCode, int i) {
    if (i < 0 || i >= particleMaxActives[spriteCode]) {
        SDL_Log("WARNING, invalid index for removedParticle removeParticle!\n");
        return;
    }
    Particle *removedParticle = activeParticles[spriteCode][i];
    if (removedParticle == nullptr) return;
    if (removedParticle->onRemove != nullptr) removedParticle->onRemove(removedParticle);

    int lastIndex = activeCounts[spriteCode] - 1;
    removedParticle->setRGBAMod(COLOR_WHITE_FULL);
    delete removedParticle;
    activeCounts[spriteCode]--;
    activeParticles[spriteCode][i] = activeParticles[spriteCode][lastIndex];
    activeParticles[spriteCode][lastIndex] = nullptr;
}

void Particle::animateAll() {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];

            currParticle->animateSprite();

            if (currParticle->fadeParams.baseAlpha != -1) {
                Uint8 alpha;
                SDL_GetTextureAlphaMod(currParticle->imgTexture, &alpha);
                alpha -= (int) ((double) Global::dt * currParticle->fadeParams.speed *
                                ((double) currParticle->fadeParams.baseAlpha / 255.0));
                if (alpha < 20) {
                    removeParticle(spriteCode, i);
                    continue;
                }
                currParticle->setRGBAMod(COLOR_WHITE, alpha);
                continue;
            }

            if (currParticle->isFinished()) {
                removeParticle(spriteCode, i);
            }

        }
    }
}

void Particle::renderAll() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            gWindow->renderEntity(currParticle);
            if (currParticle->onRender != nullptr) currParticle->onRender(currParticle);
        }
    }
}

void Particle::renderAllDebug(SDL_Renderer *gRenderer) {
    Particle *currParticle;
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            currParticle = activeParticles[spriteCode][i];
            currParticle->renderHitBox(gRenderer);
        }
    }
}

Particle *Particle::getParticle(int spriteCode, int i) {
    return (activeParticles[spriteCode][i]);
}

bool Particle::isActive(int spriteCode, int i) {
    return (activeCounts[spriteCode] > i);
}

void Particle::removeAllParticles() {
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        int count = activeCounts[spriteCode];
        for (int i = 0; i < count; i++) {
            delete activeParticles[spriteCode][i];
            activeParticles[spriteCode][i] = nullptr;
        }
        activeCounts[spriteCode] = 0;
    }
}

void Particle::cleanUp() {
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            delete activeParticles[spriteCode][i];
        }
    }

    delete baseParticle;
}

void Particle::setEntity(Entity *newEntity) {
    moveToEntityCenter(newEntity);
    facingEast = newEntity->isFacingEast();
    entity = newEntity;
    entityLastX = entity->getX();
    entityLastY = entity->getY();
}

void Particle::getRealSize(double *pW, double *pH) {
    Entity::getRealSize(pW, pH);
    if (entity != nullptr) {
        if (pW != nullptr)
            *pW *= entity->getRenderWMultiplier();
        if (pH != nullptr)
            *pH *= entity->getRenderHMultiplier();
    }
}

bool Particle::shouldTranslate() {
    return (particleCode < PARTICLE_HUD_START);
}

void Particle::animateSprite() {
    if (entity != nullptr) { // Translate Particle
        double entityX = entity->getX();
        double entityY = entity->getY();
        x += entityX - entityLastX;
        y += entityY - entityLastY;
        entityLastX = entityX;
        entityLastY = entityY;

        facingEast = entity->isFacingEast();
    }

    AnimatedEntity::animateSprite();
}

void Particle::xyShift(double xShift, double yShift) {
    if (entity != nullptr) {
        xShift *= entity->getRenderWMultiplier();
        yShift *= entity->getRenderHMultiplier();
        if (!entity->isFacingEast()) xShift = -xShift;
    } else {
        if (!isFacingEast()) xShift = -xShift;
    }
    x += xShift;
    y += yShift;
}

bool Particle::isFinished() {
    return (!getSprite()->sAnimated);
}

void Particle::fadeAway(double speed) {
    Uint8 alpha;
    SDL_GetTextureAlphaMod(imgTexture, &alpha);
    fadeParams.baseAlpha = alpha;
    fadeParams.speed = speed;
}
