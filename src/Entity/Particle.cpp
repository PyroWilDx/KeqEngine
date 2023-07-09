//
// Created by pyrowildx on 06/06/23.
//

#include "Entity/Particle.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"

Particle *Particle::baseParticle = new Particle();

int Particle::particleMaxActives[PARTICLE_ENUM_N];
Particle **Particle::activeParticles[PARTICLE_ENUM_N];
int Particle::activeCounts[PARTICLE_ENUM_N];

Particle::Particle()
        : AnimatedEntity(PARTICLE_ENUM_N), fadeParams({-1, 1}) {
    code = 0;
    entity = nullptr;
    entityLastX = 0;
    entityLastY = 0;
    onRemove = nullptr;
}

Particle::Particle(int spriteCode, int frameLength, double wMultiplier, double hMultiplier)
        : Particle() {
    for (int i = 0; i < PARTICLE_ENUM_N; i++) {
        this->setSpriteAnimated(i, false);
    }
    code = spriteCode;
    Sprite *baseSprite = baseParticle->getSprite(code);
    Sprite *currentSprite = getSprite(code);
    *currentSprite = *baseSprite;
    this->setSpriteAnimated(code, true);
    setSpriteFrameLengthFromTo(spriteCode, frameLength);
    if (baseSprite->sNext != nullptr) { // If != nullptr, then repeat itself
        currentSprite->sNext = currentSprite;
    }
    renderWMultiplier = wMultiplier;
    renderHMultiplier = hMultiplier;
    imgFrame = {0, 0, currentSprite->sFrameW, currentSprite->sFrameH};
}

void Particle::initParticle() {
    baseParticle->initSprite(PARTICLE_KQ_NATK_4, "res/gfx/particle/KQNAtk4.png",
                             80, 32, 6, 60);
    particleMaxActives[PARTICLE_KQ_NATK_4] = 1;

    baseParticle->initSprite(PARTICLE_KQ_CATK, "res/gfx/particle/KQCAtk.png",
                             160, 96, 6, 60);
    particleMaxActives[PARTICLE_KQ_CATK] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_NATK, "res/gfx/particle/KQAirNAtk.png",
                             32, 80, 4, 0);
    baseParticle->setSpriteNext(PARTICLE_KQ_AIR_NATK, PARTICLE_KQ_AIR_NATK);
    particleMaxActives[PARTICLE_KQ_AIR_NATK] = 1;

    baseParticle->initSprite(PARTICLE_KQ_AIR_NATK_GROUND, "res/gfx/particle/KQAirNAtkGround.png",
                             192, 96, 6, 0);
    particleMaxActives[PARTICLE_KQ_AIR_NATK_GROUND] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_SPAWN, "res/gfx/particle/KQSkillSpawn.png",
                             96, 96, 8, 0);
    particleMaxActives[PARTICLE_KQ_SKILL_SPAWN] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_IDLE, "res/gfx/particle/KQSkillIdle.png",
                             32, 32, 4, 0);
    baseParticle->setSpriteNext(PARTICLE_KQ_SKILL_IDLE, PARTICLE_KQ_SKILL_IDLE);
    particleMaxActives[PARTICLE_KQ_SKILL_IDLE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_TP_END, "res/gfx/particle/KQSkillTPEnd.png",
                             192, 160, 7, 0);
    particleMaxActives[PARTICLE_KQ_SKILL_TP_END] = 1;

    baseParticle->initSprite(PARTICLE_KQ_SKILL_EXPLOSION, "res/gfx/particle/KQSkillExplosion.png",
                             192, 160, 14, 0);
    particleMaxActives[PARTICLE_KQ_SKILL_EXPLOSION] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_AOE, "res/gfx/particle/KQBurstAOE.png",
                             200, 200, 7, 0);
    baseParticle->setSpriteNext(PARTICLE_KQ_BURST_AOE, PARTICLE_KQ_BURST_AOE);
    particleMaxActives[PARTICLE_KQ_BURST_AOE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_AOE_WAVE, "res/gfx/particle/KQBurstAOEWave.png",
                             200, 200, 12, 0);
    baseParticle->setSpriteNext(PARTICLE_KQ_BURST_AOE_WAVE, PARTICLE_KQ_BURST_AOE_WAVE);
    particleMaxActives[PARTICLE_KQ_BURST_AOE_WAVE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_VANISH, "res/gfx/particle/KQBurstVanish.png",
                             96, 96, 6, 0);
    particleMaxActives[PARTICLE_KQ_BURST_VANISH] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE, "res/gfx/particle/KQBurstClone.png",
                             208, 128, 23, 0);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE] = 1;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_APPEAR, "res/gfx/particle/KQBurstCloneAppear.png",
                             96, 96, 7, 0);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_APPEAR] = KQ_BURST_NUMBER_OF_CLONE;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_SLASH, "res/gfx/particle/KQBurstCloneSlash.png",
                             224, 64, 7, 0);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_SLASH] = 6;

    baseParticle->initSprite(PARTICLE_KQ_BURST_SLASH, "res/gfx/particle/KQBurstSlash.png",
                             448, 32, 7, 0);
    particleMaxActives[PARTICLE_KQ_BURST_SLASH] = KQ_BURST_NUMBER_OF_SLASH;

    baseParticle->initSprite(PARTICLE_KQ_BURST_CLONE_VANISH, "res/gfx/particle/KQBurstCloneVanish.png",
                             96, 96, 4, 0);
    particleMaxActives[PARTICLE_KQ_BURST_CLONE_VANISH] = KQ_BURST_NUMBER_OF_CLONE;

    baseParticle->initSprite(PARTICLE_KQ_BURST_FINAL_SLASH, "res/gfx/particle/KQBurstFinalSlash.png",
                             800, 320, 10, 0);
    particleMaxActives[PARTICLE_KQ_BURST_FINAL_SLASH] = 1;

    particleMaxActives[PARTICLE_HUD_START] = 0;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_CIRCLE_BG, "res/gfx/hud/SkillBurstCircleBG.png",
                             32, 32, 2, 0);
    particleMaxActives[PARTICLE_HUD_SKILL_CIRCLE_BG] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_CIRCLE_BG, "res/gfx/hud/SkillBurstCircleBG.png",
                             32, 32, 2, 0);
    baseParticle->goToFrame(PARTICLE_HUD_BURST_CIRCLE_BG, 1);
    particleMaxActives[PARTICLE_HUD_BURST_CIRCLE_BG] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_CIRCLE, "res/gfx/hud/SkillBurstCircle.png",
                             32, 32, 2, 0);
    particleMaxActives[PARTICLE_HUD_SKILL_CIRCLE] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_CIRCLE, "res/gfx/hud/SkillBurstCircle.png",
                             32, 32, 2, 0);
    baseParticle->goToFrame(PARTICLE_HUD_BURST_CIRCLE, 1);
    particleMaxActives[PARTICLE_HUD_BURST_CIRCLE] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_ICON_1, "res/gfx/hud/SkillIcon1.png",
                             21, 21, 1, 0);
    particleMaxActives[PARTICLE_HUD_SKILL_ICON_1] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_ICON_2, "res/gfx/hud/SkillIcon2.png",
                             21, 21, 1, 0);
    particleMaxActives[PARTICLE_HUD_SKILL_ICON_2] = 1;

    baseParticle->initSprite(PARTICLE_HUD_BURST_ICON, "res/gfx/hud/BurstIcon.png",
                             21, 21, 1, 0);
    particleMaxActives[PARTICLE_HUD_BURST_ICON] = 1;

    baseParticle->initSprite(PARTICLE_HUD_SKILL_BURST_TIMER, "res/gfx/hud/SkillBurstTimer.png",
                             32, 32, HUD_SB_TIMER_FRAME_N, 0);
    particleMaxActives[PARTICLE_HUD_SKILL_BURST_TIMER] = 2;

    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        activeParticles[spriteCode] = (Particle **)
                calloc(particleMaxActives[spriteCode], sizeof(Particle *));
        activeCounts[spriteCode] = 0;
    }
}

Particle *Particle::push(int spriteCode, int frameLength, double wMultiplier, double hMultiplier) {
    auto *particle = new Particle(spriteCode, frameLength, wMultiplier, hMultiplier);
    pushFast(particle);
    return particle;
}

void Particle::pushFast(Particle *particle) {
    int spriteCode = particle->code;
    int count = activeCounts[spriteCode];
    int i;
    if (count < particleMaxActives[spriteCode]) {
        i = count;
    } else { // Replace existing Particle (last one)
        i = count - 1;
        remove(spriteCode, i);
    }
    activeParticles[spriteCode][i] = particle;
    activeCounts[spriteCode]++;
}

void Particle::remove(int spriteCode, int i) {
    if (i < 0 || i >= particleMaxActives[spriteCode]) {
        SDL_Log("WARNING, invalid index for removedParticle remove!\n");
        return;
    }
    Particle *removedParticle = activeParticles[spriteCode][i];
    if (removedParticle == nullptr) return;
    if (removedParticle->onRemove != nullptr) removedParticle->onRemove(removedParticle);

    int lastIndex = activeCounts[spriteCode] - 1;
    removedParticle->setRGBAMod(RGBA_FULL);
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

            if (currParticle->entity != nullptr) { // Translate Particle
                double entityX = currParticle->entity->getX();
                double entityY = currParticle->entity->getY();
                currParticle->x += entityX - currParticle->entityLastX;
                currParticle->y += entityY - currParticle->entityLastY;
                currParticle->entityLastX = entityX;
                currParticle->entityLastY = entityY;

                currParticle->facingEast = currParticle->entity->isFacingEast();
            }

            currParticle->animateSprite();

            if (currParticle->fadeParams.baseAlpha != -1) {
                Uint8 alpha;
                SDL_GetTextureAlphaMod(currParticle->imgTexture, &alpha);
                alpha -= (int) ((double) Global::dt * currParticle->fadeParams.speed *
                                ((double) currParticle->fadeParams.baseAlpha / 255.0));
                if (alpha < 20) {
                    remove(spriteCode, i);
                    continue;
                }
                currParticle->setRGBAMod(RGB_FULL, alpha);
                continue;
            }

            if (currParticle->isFinished()) {
                remove(spriteCode, i);
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

Particle *Particle::getParticle(int spriteCode, int i) {
    return (activeParticles[spriteCode][i]);
}

bool Particle::isActive(int spriteCode, int i) {
    return (activeCounts[spriteCode] > i);
}

void Particle::cleanUp() {
    for (int i = 0; i < PARTICLE_ENUM_N; i++) {
        if (particleMaxActives[i] == 0) continue;
        SDL_DestroyTexture(baseParticle->getSprite(i)->sTexture);
    }
    for (int spriteCode = 0; spriteCode < PARTICLE_ENUM_N; spriteCode++) {
        for (int i = 0; i < activeCounts[spriteCode]; i++) {
            delete activeParticles[spriteCode][i];
        }
    }
}

void Particle::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Texture *currTexture = getSprite(code)->sTexture;
    SDL_SetTextureColorMod(currTexture, r, g, b);
    SDL_SetTextureAlphaMod(currTexture, a);
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
    return (code < PARTICLE_HUD_START);
}

void Particle::xyShift(double xShift, double yShift) {
    if ((entity != nullptr && !entity->isFacingEast()) ||
        !isFacingEast()) {
        x -= xShift;
    } else {
        x += xShift;
    }
    y += yShift;
}

bool Particle::isFinished() {
    return (!getSprite(code)->sAnimated);
}

void Particle::fadeAway(double speed) {
    Uint8 alpha;
    SDL_GetTextureAlphaMod(imgTexture, &alpha);
    fadeParams.baseAlpha = alpha;
    fadeParams.speed = speed;
}

Particle *Particle::copy() {
    auto *newParticle = new Particle(code,
                                     getSprite(code)->sFrameLengths[0],
                                     renderWMultiplier,
                                     renderHMultiplier);
    newParticle->x = x;
    newParticle->y = y;
    newParticle->imgFrame = imgFrame;
    newParticle->degRotation = degRotation;
    newParticle->entity = entity;
    newParticle->facingEast = facingEast;

    return newParticle;
}