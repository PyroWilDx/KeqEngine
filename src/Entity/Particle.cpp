//
// Created by pyrowildx on 06/06/23.
//

#include "Entity/Particle.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"
#include "WindowRenderer.hpp"
#include "StructForEntity/Attack.hpp"

Particle *Particle::baseParticle = new Particle(true);

int Particle::particleMaxActives[PARTICLE_ENUM_N];
Particle **Particle::activeParticles[PARTICLE_ENUM_N];
int Particle::activeCounts[PARTICLE_ENUM_N];

Particle::Particle(bool isBaseParticle)
        : AnimatedEntity((isBaseParticle) ? PARTICLE_ENUM_N : 1),
          fadeParams({-1, 1}) {
    particleCode = -1;
    entity = nullptr;
    entitySizeDependant = true;
    entityLastX = 0;
    entityLastY = 0;
    onRender = nullptr;
    onRenderParams = nullptr;
    onRemove = nullptr;
    if (isBaseParticle) imgTexture = getSprite()->sTexture;
}

Particle::Particle(int spriteCode, int frameLength, double wMultiplier, double hMultiplier)
        : Particle(false) {
    this->particleCode = spriteCode;
    Sprite *baseSprite = baseParticle->getSprite(spriteCode);
    Sprite *currentSprite = getSprite();
    *currentSprite = *baseSprite;
    currentSprite->sCode = 0;
    currentSprite->sTexture = WindowRenderer::getInstance()->loadTexture(
            currentSprite->imgPath);
    AnimatedEntity::setSpriteAnimated(true);
    currentSprite->sFrameLengths = new int[currentSprite->sFrameN];
    setSpriteFrameLengthFromTo(frameLength, 0, -1);
    if (baseSprite->sNext != nullptr) { // If != NULL, then repeat itself
        currentSprite->sNext = currentSprite;
    }
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    imgFrame = {0, 0, currentSprite->sFrameW, currentSprite->sFrameH};
    imgTexture = currentSprite->sTexture;
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

    baseParticle->initSprite(PARTICLE_KQ_DASH_VANISH, "res/gfx/particle/KQDashVanish.png",
                             128, 128, 7);
    particleMaxActives[PARTICLE_KQ_DASH_VANISH] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_PLUNGE, "res/gfx/particle/KQAirPlunge.png",
                             32, 80, 4);
    baseParticle->setSpriteNext(PARTICLE_KQ_AIR_PLUNGE, PARTICLE_KQ_AIR_PLUNGE);
    particleMaxActives[PARTICLE_KQ_AIR_PLUNGE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_PLUNGE_GROUND, "res/gfx/particle/KQAirPlungeGround.png",
                             192, 96, 6);
    particleMaxActives[PARTICLE_KQ_AIR_PLUNGE_GROUND] = 1;

    baseParticle->initSprite(PARTICLE_KQ_ELECTRO_AURA, "res/gfx/particle/KQElectroAura.png",
                             96, 96, 8);
    particleMaxActives[PARTICLE_KQ_ELECTRO_AURA] = 256;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_BLINK_0, "res/gfx/particle/KQSkillBlink0.png",
                             96, 96, 4);
    particleMaxActives[PARTICLE_KQ_SKILL_BLINK_0] = 2;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_BLINK_1, "res/gfx/particle/KQSkillBlink1.png",
                             96, 96, 4);
    particleMaxActives[PARTICLE_KQ_SKILL_BLINK_1] = 2;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_FLIP_SLASH, "res/gfx/particle/KQSkillFlipSlash.png",
                             128, 128, 5);
    particleMaxActives[PARTICLE_KQ_SKILL_FLIP_SLASH] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CLONE_FADE, "res/gfx/particle/KQSkillCloneFade.png",
                             96, 96, 4);
    particleMaxActives[PARTICLE_KQ_SKILL_CLONE_FADE] = 8;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CLONE_SPAWN, "res/gfx/particle/KQSkillCloneSpawn.png",
                             96, 96, 6);
    particleMaxActives[PARTICLE_KQ_SKILL_CLONE_SPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CLONE_IDLE, "res/gfx/particle/KQSkillCloneIdle.png",
                             96, 96, 4);
    particleMaxActives[PARTICLE_KQ_SKILL_CLONE_IDLE] = 1;
    baseParticle->setSpriteNext(PARTICLE_KQ_SKILL_CLONE_IDLE, PARTICLE_KQ_SKILL_CLONE_IDLE);

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CLONE_DESPAWN, "res/gfx/particle/KQSkillCloneDespawn.png",
                             96, 96, 7);
    particleMaxActives[PARTICLE_KQ_SKILL_CLONE_DESPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CURSOR_SPAWN, "res/gfx/particle/KQSkillCursorSpawn.png",
                             96, 96, 7);
    particleMaxActives[PARTICLE_KQ_SKILL_CURSOR_SPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_CURSOR, "res/gfx/particle/KQSkillCursor.png",
                             48, 48, 8);
    baseParticle->setSpriteNext(PARTICLE_KQ_SKILL_CURSOR, PARTICLE_KQ_SKILL_CURSOR);
    particleMaxActives[PARTICLE_KQ_SKILL_CURSOR] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_SHOOT, "res/gfx/particle/KQSkillShoot.png",
                             64, 64, 8);
    particleMaxActives[PARTICLE_KQ_SKILL_SHOOT] = 1;

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

    baseParticle->initSprite(PARTICLE_KQ_SKILL_DESPAWN, "res/gfx/particle/KQSkillDespawn.png",
                             96, 96, 8);
    particleMaxActives[PARTICLE_KQ_SKILL_DESPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_TP_START, "res/gfx/particle/KQSkillTPStart.png",
                             192, 160, 8);
    particleMaxActives[PARTICLE_KQ_SKILL_TP_START] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_TP_END, "res/gfx/particle/KQSkillTPEnd.png",
                             192, 160, 7);
    particleMaxActives[PARTICLE_KQ_SKILL_TP_END] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_EXPLOSION, "res/gfx/particle/KQSkillExplosion.png",
                             192, 160, 14);
    particleMaxActives[PARTICLE_KQ_SKILL_EXPLOSION] = 1;

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

    baseParticle->initSprite(PARTICLE_DMG_PHYSICAL_0, "res/gfx/particle/DMGPhysical0.png",
                             96, 96, 8);
    particleMaxActives[PARTICLE_DMG_PHYSICAL_0] = 16;

    baseParticle->initSprite(PARTICLE_DMG_PHYSICAL_1, "res/gfx/particle/DMGPhysical1.png",
                             128, 128, 8);
    particleMaxActives[PARTICLE_DMG_PHYSICAL_1] = particleMaxActives[PARTICLE_DMG_PHYSICAL_0];

    baseParticle->initSprite(PARTICLE_DMG_PHYSICAL_2, "res/gfx/particle/DMGPhysical2.png",
                             160, 160, 8);
    particleMaxActives[PARTICLE_DMG_PHYSICAL_2] = particleMaxActives[PARTICLE_DMG_PHYSICAL_0];

    baseParticle->initSprite(PARTICLE_DMG_PHYSICAL_MINI, "res/gfx/particle/DMGPhysicalMini.png",
                             32, 32, 4);
    particleMaxActives[PARTICLE_DMG_PHYSICAL_MINI] =
            ATK_SMALL_PARTICLE_COUNT * particleMaxActives[PARTICLE_DMG_PHYSICAL_0];

    baseParticle->initSprite(PARTICLE_DMG_ELECTRO_0, "res/gfx/particle/DMGElectro0.png",
                             96, 96, 8);
    particleMaxActives[PARTICLE_DMG_ELECTRO_0] = 16;

    baseParticle->initSprite(PARTICLE_DMG_ELECTRO_1, "res/gfx/particle/DMGElectro1.png",
                             128, 128, 8);
    particleMaxActives[PARTICLE_DMG_ELECTRO_1] = particleMaxActives[PARTICLE_DMG_ELECTRO_0];

    baseParticle->initSprite(PARTICLE_DMG_ELECTRO_2, "res/gfx/particle/DMGElectro2.png",
                             160, 160, 8);
    particleMaxActives[PARTICLE_DMG_ELECTRO_2] = particleMaxActives[PARTICLE_DMG_ELECTRO_0];

    baseParticle->initSprite(PARTICLE_DMG_ELECTRO_MINI, "res/gfx/particle/DMGElectroMini.png",
                             32, 32, 4);
    particleMaxActives[PARTICLE_DMG_ELECTRO_MINI] =
            ATK_SMALL_PARTICLE_COUNT * particleMaxActives[PARTICLE_DMG_ELECTRO_0];

    baseParticle->initSprite(PARTICLE_EXPLOSION, "res/gfx/particle/Explosion.png",
                             64, 64, 48);
    particleMaxActives[PARTICLE_EXPLOSION] = 1;

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
        SDL_Log("WARNING, invalid index for removedParticle removeParticle (%d) !", i);
        return;
    }
    Particle *removedParticle = activeParticles[spriteCode][i];
    if (removedParticle == nullptr) return;
    if (removedParticle->onRemove != nullptr) removedParticle->onRemove(removedParticle);

    int lastIndex = activeCounts[spriteCode] - 1;
    if (activeCounts[spriteCode] == 1) removedParticle->setRGBAMod(COLOR_MAX);
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

            currParticle->onGameFrame();

            currParticle->animateSprite();

            if (currParticle->fadeParams.baseAlpha != -1) {
                Uint8 alpha;
                SDL_GetTextureAlphaMod(currParticle->imgTexture, &alpha);
                alpha -= (int) ((double) Global::dt * currParticle->fadeParams.speed *
                                ((double) currParticle->fadeParams.baseAlpha / COLOR_MAX_DBL));
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
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            gWindow->renderEntity(activeParticles[spriteCode][i]);
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

void Particle::hardResetOnRender(int spriteCode) {
    for (int i = 0; i < activeCounts[spriteCode]; i++) {
        activeParticles[spriteCode][i]->onRender = nullptr;
    }
}

void Particle::removeAllParticles() {
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        int count = activeCounts[spriteCode];
        for (int i = 0; i < count; i++) {
            Particle *removedParticle = activeParticles[spriteCode][i];
            if (removedParticle->onRenderParams != nullptr) removedParticle->onRemove(removedParticle);
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

void Particle::getRealSize(double *pW, double *pH) {
    Entity::getRealSize(pW, pH);
    if (entity != nullptr && entitySizeDependant) {
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

void Particle::setEntity(Entity *newEntity, bool sizeDependant,
                         bool takeFaceEast) {
    entity = newEntity;
    setEntitySizeDependant(sizeDependant);
    if (entity != nullptr) {
        moveToEntityCenter(entity, takeFaceEast);
        entityLastX = entity->getX();
        entityLastY = entity->getY();
    }
}

void Particle::setParticleEntity(Particle *srcParticle, bool sizeDependant,
                                 bool takeFaceEast) {
    setEntity(srcParticle->entity, sizeDependant, takeFaceEast);
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

void Particle::renderSelf(SDL_Renderer *gRenderer) {
    AnimatedEntity::renderSelf(gRenderer);

    if (onRender != nullptr) onRender(this);
}

void Particle::removeSelf() {
    for (int i = 0; i < activeCounts[particleCode]; i++) {
        if (this == activeParticles[particleCode][i]) {
            Particle::removeParticle(particleCode, i);
            break;
        }
    }
}

Particle *Particle::cloneSelf(int onRenderParamsSize) {
    auto *newParticle = new Particle(particleCode,
                                     0,
                                     renderWMultiplier,
                                     renderHMultiplier);
    std::memcpy(newParticle->getSprite()->sFrameLengths,
                getSprite()->sFrameLengths,
                getSprite()->sFrameN * sizeof(getSprite()->sFrameLengths[0]));
    newParticle->moveTo(x, y);
    newParticle->setXYVelocity(xVelocity, yVelocity);
    newParticle->entity = entity;
    newParticle->entityLastX = entityLastX;
    newParticle->entityLastY = entityLastY;
    newParticle->fadeParams = fadeParams;
    newParticle->onRender = onRender;
    if (onRenderParams != nullptr && onRenderParamsSize > 0) {
        newParticle->onRenderParams = (void *) malloc(onRenderParamsSize);
        memcpy(newParticle->onRenderParams, onRenderParams,
               onRenderParamsSize);
    }
    newParticle->onRemove = onRemove;

    return newParticle;
}
