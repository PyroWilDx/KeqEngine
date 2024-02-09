//
// Created by pyrowildx on 25/06/2023.
//

#include "World/World.hpp"
#include "Utils/Events.hpp"
#include "Keqing.hpp"
#include "Utils/Sound.hpp"
#include "Entity/Particle.hpp"
#include "WindowRenderer.hpp"
#include "World/Background.hpp"
#include "World/Block.hpp"
#include "UI/Button.hpp"
#include "EntityInfo/Attack.hpp"
#include "AbstractEntity/Monster.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Global.hpp"
#include "Entity/DamageText.hpp"

World::World(int screenW, int screenH,
             int backgroundTotalW, int backgroundTotalH,
             const char *backgroundImgPath)
        : colorFilter({COLOR_BLACK, 0, 0, false}) {
    this->background = new Background(screenW, screenH,
                                      backgroundTotalW, backgroundTotalH,
                                      backgroundImgPath);
    this->translateBackgroundEntity = nullptr;

    this->activeButton = nullptr;

    this->renderKeqing = false;

    this->kqAtkLL = nullptr;
    this->monsterAtkLL = nullptr;

    this->colorFilterSpeed = 0.16;

    this->displayMenu = false;

    this->pixels = new Pixel *[backgroundTotalW];
    for (int i = 0; i < background->getTotalW(); i++) {
        (this->pixels)[i] = new Pixel[backgroundTotalH];
        for (int j = 0; j < backgroundTotalH; j++) {
            (this->pixels)[i][j] = {-1, -1};
        }
    }

    stopSoundOnQuit = STOP_SOUND_ALL;

    onQuit = nullptr;
}

auto LLFreeAtkF = [](void *value) {
    auto *atk = (Attack *) value;
    delete atk;
};

World::~World() {
    if (onQuit != nullptr) onQuit();

    for (std::pair<const int, Button *> it: buttonHashMap) {
        delete it.second;
    }

    for (Block *block: blockVector) {
        delete block;
    }
    for (Monster *monster: monsterVector) {
        delete monster;
    }
    for (Entity *entity: otherEntityVecotr) {
        delete entity;
    }

    LLFree(monsterAtkLL, LLFreeAtkF);
    LLFree(kqAtkLL, LLFreeAtkF);

    for (int i = 0; i < background->getTotalW(); i++) {
        delete[] pixels[i];
    }
    delete[] pixels;

    switch (stopSoundOnQuit) {
        case STOP_SOUND_CHUNK:
            Sound::stopAllAudioChunks();
            break;

        case STOP_SOUND_MUSIC:
            Sound::deleteAudioMusic();
            break;

        case STOP_SOUND_ALL:
            Sound::stopAllSounds();
            break;

        default:
            break;
    }

    delete background;
}

void World::updatePixels(int x1, int y1, int x2, int y2, WorldEntity *worldEntity) {
    for (int i = x1; i < x2; i++) {
        for (int j = y1; j < y2; j++) {
            pixels[i][j] = {worldEntity->getWorldEntityType(), worldEntity->getWorldCode()};
        }
    }
}

void World::updatePixels(SDL_Rect *rect, WorldEntity *worldEntity) {
    updatePixels(rect->x, rect->y, rect->w, rect->h, worldEntity);
}

void World::refreshPixelsOnRemove(WorldEntity *worldEntity) {
    for (int i = worldEntity->getX(); i < worldEntity->getX() + worldEntity->getRenderW(); i++) {
        for (int j = worldEntity->getY(); j < worldEntity->getY() + worldEntity->getRenderW(); j++) {
            pixels[i][j] = {WORLD_BACKGROUND, -1};
        }
    }
    SDL_Rect intersectArea;
    for (Block *block: blockVector) {
        if (block->getCollisionArea(worldEntity, &intersectArea)) {
            updatePixels(&intersectArea, worldEntity);
        }
    }
}

bool World::xyOutOfBounds(double x, double y) {
    return (x < 0 || x >= background->getTotalW() ||
            y < 0 || y >= background->getTotalH());
}

Pixel World::getPixel(double x, double y) {
    if (xyOutOfBounds(x, y)) {
        return {WORLD_BLOCK, BLOCK_WALL_INVISIBLE};
    }
    return pixels[(int) x][(int) y];
}

bool World::isPixelCode(double x, double y, int worldCode) {
    return (getPixel(x, y).worldCode == worldCode);
}

void World::addWorldEntity(WorldEntity *worldEntity) {
    double worldEntityW, worldEntityH;
    worldEntity->getRealSize(&worldEntityW, &worldEntityH);
    int minX = std::max(worldEntity->getX(), 0);
    int maxX = (int) std::min(worldEntity->getX() + worldEntityW, (double) background->getTotalW());
    int minY = std::max(worldEntity->getY(), 0);
    int maxY = (int) std::min(worldEntity->getY() + worldEntityH, (double) background->getTotalH());
    updatePixels(minX, minY, maxX, maxY, worldEntity);
}

void World::addButton(Button *button) {
    buttonHashMap[button->getWorldCode()] = button;
    addWorldEntity(button);
}

bool World::isPixelButton(double x, double y) {
    return (getPixel(x, y).worldType == WORLD_BUTTON);
}

void World::clickPixel(double x, double y, Uint32 eventType) {
    if (eventType == SDL_MOUSEBUTTONDOWN) {
        if (isPixelButton(x, y)) {
            int worldCode = getPixel(x, y).worldCode;
            auto buttonIterator = buttonHashMap.equal_range(worldCode);
            for (auto it = buttonIterator.first; it != buttonIterator.second; it++) {
                Button *button = it->second;
                if (button->getWorldCode() == worldCode) {
                    button->onClick((int) x, (int) y);
                    activeButton = button;
                    break;
                }
            }
        }
        return;
    }

    if (eventType == SDL_MOUSEMOTION) {
        if (activeButton != nullptr) {
            bool isMouseOnButton = isPixelCode(x, y, activeButton->getWorldCode());
            activeButton->onClickedMove((int) x, (int) y, isMouseOnButton);
        }
        return;
    }

    if (eventType == SDL_MOUSEBUTTONUP) {
        if (activeButton != nullptr) {
            bool isMouseOnButton = isPixelCode(x, y, activeButton->getWorldCode());
            Button *tmpButton = activeButton; // Because World May be Deleted !
            activeButton = nullptr;
            tmpButton->onClickRelease((int) x, (int) y, isMouseOnButton);
        }
        return;
    }
}

void World::addBlock(Block *block) {
    blockVector.push_back(block);
    addWorldEntity(block);
}

Block *World::addBlock(int blockCode, double x, double y, int renderW, int renderH) {
    auto *block = new Block(blockCode, x, y, renderW, renderH);
    addBlock(block);
    return block;
}

Block *World::addBlock(int blockCode, double x, double y, int renderW) {
    return addBlock(blockCode, x, y, renderW, background->getTotalH() - (int) y);
}

void World::addCoveredBlock(int blockCode, int coverBlockCode, double x, double y,
                            int renderW) {
    auto *block = new Block(blockCode, x, y + 32.,
                            renderW, background->getTotalH() - ((int) y) - 32);
    auto *coverBlock = new Block(coverBlockCode, x, y, renderW, 32);
    coverBlock->resizeToRenderSizeOnY();
    addBlock(block);
    addBlock(coverBlock);
}

void World::removeBlock(Block **block) {
    removePointerElementFromVector(*block, &blockVector);
    refreshPixelsOnRemove(*block);
    delete *block;
    *block = nullptr;
}

bool World::isPixelBlock(double x, double y) {
    return (getPixel(x, y).worldType == WORLD_BLOCK);
}

bool World::isPixelSurface(double x, double y) {
    return (isPixelBlock(x, y));
}

int World::getNearestWallFrom(double x, double y, int direction) {
    int addX = 0;
    int addY = 0;
    if (direction == KEY_Z) {
        addX = 0;
        addY = -1;
    } else if (direction == KEY_Q) {
        addX = -1;
        addY = 0;
    } else if (direction == KEY_S) {
        addX = 0;
        addY = 1;
    } else if (direction == KEY_D) {
        addX = 1;
        addY = 0;
    }

    if (x < 0) x = 0;
    if (x >= background->getTotalW()) x = background->getTotalW() - 1;
    if (y < 0) y = 0;
    if (y > background->getTotalH()) y = background->getTotalH() - 1;
    int i = (int) x;
    int j = (int) y;
    while (isPixelSurface(i, j)) {
        i += addX;
        j += addY;
        if (xyOutOfBounds(i, j)) {
            return GET_NEAREST_WALL_RETURN_NONE;
        }
    }
    if (addX == 0) return (j - addY);
    else return (i - addX);
}

void World::addMonster(Monster *monster) {
    monsterVector.push_back(monster);
}

void World::removeMonster(Monster *monster) {
    monsterVector.erase(
            std::remove(monsterVector.begin(), monsterVector.end(), monster),
            monsterVector.end());
    delete monster;
}

void World::addDamageText(DamageText *dmgText) {
    dmgTextSet.insert(dmgText);
}

void World::removeDamageText(DamageText *dmgText) {
    dmgTextSet.erase(dmgText);
    delete dmgText;
}

void World::addOtherEntity(Entity *otherEntity) {
    otherEntityVecotr.push_back(otherEntity);
}

void World::addMenuEntity(Entity *menuEntity) {
    menuEntityVector.push_back(menuEntity);
}

void World::addKQAtk(Attack *atk, double atkPercent) {
    Keqing *kq = Keqing::getInstance();
    atk->setAtkDamage((int) (atkPercent * kq->getTotalAtk() *
                             kq->getBonusDmgMultiplier() * kq->getCritDmg()));
    kqAtkLL = LLInsertHead(kqAtkLL, (void *) atk);
}

Attack *World::addKQAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                        double atkPercent, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, xyArray, arrayLength,
                           0, kbXVelocity, kbYVelocity);
    addKQAtk(atk, atkPercent);
    return atk;
}

Attack *World::addKQAtk(LivingEntity *atkIssuer, Entity *followEntity,
                        double xyArray[][2], int arrayLength,
                        double atkPercent, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, followEntity,
                           xyArray, arrayLength,
                           0, kbXVelocity, kbYVelocity);
    addKQAtk(atk, atkPercent);
    return atk;
}

void World::addMonsterAtk(Attack *atk) {
    monsterAtkLL = LLInsertHead(monsterAtkLL, (void *) atk);
}

Attack *World::addMonsterAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                             int damage, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, xyArray, arrayLength,
                           damage, kbXVelocity, kbYVelocity);
    addMonsterAtk(atk);
    return atk;
}

Attack *World::addMonsterAtk(LivingEntity *atkIssuer, Entity *followEntity,
                             double xyArray[][2], int arrayLength,
                             int damage, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, followEntity,
                           xyArray, arrayLength,
                           damage, kbXVelocity, kbYVelocity);
    addMonsterAtk(atk);
    return atk;
}

void World::enableColorFilter(Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                              double speed) {
    if (colorFilter.filterActivated) {
        SDL_Log("Color filter has been overwritten, is it wanted ?");
    }

    colorFilterSpeed = speed;

    clearAndShrinkVector(&ignoreFilterEntityVector);

    colorFilter.r = r;
    colorFilter.g = g;
    colorFilter.b = b;

    colorFilter.currAlpha = 0;
    colorFilter.targetAlpha = a;

    colorFilter.filterActivated = true;
}

void World::disableColorFilter() {
    colorFilter.filterActivated = false;
}

void World::addIgnoreFilterEntity(Entity *ignoreFilterEntity) {
    ignoreFilterEntityVector.push_back(ignoreFilterEntity);
}

void World::onGameFrame() {
    Sound::onGameFrame();

    const int nMonster = (int) monsterVector.size();
    for (int i = 0; i < nMonster; i++) {
        monsterVector[i]->onGameFrame();
    }

    const int nOtherEntity = (int) otherEntityVecotr.size();
    for (int i = 0; i < nOtherEntity; i++) {
        otherEntityVecotr[i]->onGameFrame();
    }

    if (renderKeqing) Keqing::getInstance()->onGameFrame();

    Particle::animateAll();

    auto itDmgText = dmgTextSet.begin();
    while (itDmgText != dmgTextSet.end()) {
        (*itDmgText)->onGameFrame();
        itDmgText++;
    }

    // Attacks
    auto fAtkShouldRemove = [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        return atk->shouldSelfRemove();
    };
    kqAtkLL = LLIterateMayRemove(kqAtkLL, fAtkShouldRemove, nullptr,
                                 LLFreeAtkF);
    monsterAtkLL = LLIterateMayRemove(monsterAtkLL, fAtkShouldRemove, nullptr,
                                      LLFreeAtkF);

    const auto fAtkOnGameFrame = [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        atk->onGameFrame();
    };
    LLIterate(kqAtkLL, fAtkOnGameFrame, nullptr);
    LLIterate(monsterAtkLL, fAtkOnGameFrame, nullptr);

    LLIterate(kqAtkLL, [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        auto *monsters = (std::vector<Monster *> *) fParams;
        for (Monster *monster: *monsters) {
            atk->checkEntityHit(monster);
        }
    }, (void *) &monsterVector);

    LLIterate(monsterAtkLL, [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        auto *kq_ = (Keqing *) fParams;
        atk->checkEntityHit(kq_);
    }, (void *) Keqing::getInstance());

    if (translateBackgroundEntity != nullptr) {
        background->lerpTranslate(translateBackgroundEntity);
    }
}

void World::renderFilter() {
    double targetAlpha = colorFilter.targetAlpha;

    if (colorFilter.filterActivated && colorFilter.currAlpha < targetAlpha) {
        colorFilter.currAlpha = std::min(
                colorFilter.currAlpha + colorFilterSpeed * (double) Global::dt,
                targetAlpha);
    }

    if (!colorFilter.filterActivated && colorFilter.currAlpha > 0) {
        colorFilter.currAlpha = std::max(
                colorFilter.currAlpha - colorFilterSpeed * (double) Global::dt,
                0.);
        if (colorFilter.currAlpha == 0) {
            clearAndShrinkVector(&ignoreFilterEntityVector);
        }
    }

    if (colorFilter.currAlpha > 0) {
        SDL_Rect filterRect = {0, 0,
                               SCREEN_BASE_WIDTH,
                               SCREEN_BASE_HEIGHT};
        WindowRenderer::renderRect(&filterRect, true,
                                   colorFilter.r,
                                   colorFilter.g,
                                   colorFilter.b,
                                   (Uint8) colorFilter.currAlpha,
                                   WindowRenderer::getInstance()->getRenderer(),
                                   false, false);
    }
}

void World::renderSelf() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    gWindow->renderEntity(background);

    for (Block *block: blockVector) {
        gWindow->renderEntity(block);
    }

    for (Monster *monster: monsterVector) {
        gWindow->renderEntity(monster);
    }

    for (Entity *otherEntity: otherEntityVecotr) {
        gWindow->renderEntity(otherEntity);
    }

    if (renderKeqing) gWindow->renderEntity(Keqing::getInstance());

    Particle::renderAll();

    for (DamageText *dmgText: dmgTextSet) {
        gWindow->renderEntity(dmgText);
    }

    renderFilter();

    for (Entity *ignoreFilterEntity: ignoreFilterEntityVector) {
        gWindow->renderEntity(ignoreFilterEntity);
    }

    for (std::pair<const int, Button *> &it: buttonHashMap) {
        gWindow->renderEntity(it.second);
    }

    if (displayMenu) {
        for (Entity *menuEntity: menuEntityVector) {
            gWindow->renderEntity(menuEntity);
        }
    }
}

void World::renderDebugMode() {
    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

    for (Block *block: blockVector) {
        block->renderHitBox(gRenderer);
    }
    for (Monster *monster: monsterVector) {
        monster->renderHitBox(gRenderer);
    }
    for (Entity *entity: otherEntityVecotr) {
        entity->renderHitBox(gRenderer);
    }
    if (renderKeqing) Keqing::getInstance()->renderHitBox(gRenderer);

    Particle::renderAllDebug(gRenderer);

    auto fRenderAtk = [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        auto *gRenderer = (SDL_Renderer *) fParams;
        atk->renderSelf(gRenderer);
    };
    LLIterate(monsterAtkLL, fRenderAtk, gRenderer);
    LLIterate(kqAtkLL, fRenderAtk, gRenderer);
}

void World::onEntityRemove(Entity *removedEntity) {
    auto ptrRemovedEntity = (void *) removedEntity;

    removePointerElementFromVector(ptrRemovedEntity, &monsterVector);
    removePointerElementFromVector(ptrRemovedEntity, &otherEntityVecotr);
    removePointerElementFromVector(ptrRemovedEntity, &ignoreFilterEntityVector);
}
