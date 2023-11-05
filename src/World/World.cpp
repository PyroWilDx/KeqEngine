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
#include "StructForEntity/Attack.hpp"
#include "BaseEntity/Monster.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Global.hpp"

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

    if (renderKeqing) {
        Keqing *kq = Keqing::getInstance();
        if (kq != nullptr) kq->reset();
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

void World::addBlock(int blockCode, double x, double y, int renderW, int renderH) {
    auto *block = new Block(blockCode, x, y, renderW, renderH);
    addBlock(block);
}

bool World::isPixelBlock(double x, double y) {
    return (getPixel(x, y).worldType == WORLD_BLOCK);
}

bool World::isPixelSurface(double x, double y) {
    return (isPixelBlock(x, y));
}

int World::getNearestWallFrom(double x, double y, int direction) {
    int addX, addY;
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

void World::addOtherEntity(Entity *otherEntity) {
    otherEntityVecotr.push_back(otherEntity);
}

void World::addKQAtk(Attack *atk) {
    kqAtkLL = LLInsertHead(kqAtkLL, (void *) atk);
}

Attack *World::addKQAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                        int damage, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, xyArray, arrayLength,
                           damage, kbXVelocity, kbYVelocity);
    addKQAtk(atk);
    return atk;
}

Attack *World::addKQAtk(LivingEntity *atkIssuer, Entity *followEntity,
                        double xyArray[][2], int arrayLength,
                        int damage, double kbXVelocity, double kbYVelocity) {
    auto *atk = new Attack(atkIssuer, followEntity,
                           xyArray, arrayLength,
                           damage, kbXVelocity, kbYVelocity);
    addKQAtk(atk);
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

void World::enableColorFilter(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (colorFilter.filterActivated) {
        SDL_Log("Color filter has been overwritten, is it wanted ?");
    }

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
    Particle::animateAll();

    for (Monster *monster: monsterVector) {
        monster->onGameFrame();
    }
    for (Entity *entity: otherEntityVecotr) {
        entity->onGameFrame();
    }
    if (renderKeqing) Keqing::getInstance()->onGameFrame();

    // Attacks
    auto fAtkShouldRemove = [](void *value, void *fParams) {
        auto *atk = (Attack *) value;
        return atk->shouldSelfRemove();
    };
    kqAtkLL = LLIterateMayRemove(kqAtkLL, fAtkShouldRemove, nullptr,
                                 LLFreeAtkF);
    monsterAtkLL = LLIterateMayRemove(monsterAtkLL, fAtkShouldRemove, nullptr,
                                      LLFreeAtkF);

    auto fAtkOnGameFrame = [](void *value, void *fParams) {
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
        background->translate(translateBackgroundEntity);
    }
}

void World::renderFilter() {
    const double colorFilterSpeed = 0.16;

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
    for (std::pair<const int, Button *> &it: buttonHashMap) {
        gWindow->renderEntity(it.second);
    }
    for (Monster *monster: monsterVector) {
        gWindow->renderEntity(monster);
    }
    for (Entity *otherEntity: otherEntityVecotr) {
        gWindow->renderEntity(otherEntity);
    }
    if (renderKeqing) gWindow->renderEntity(Keqing::getInstance());

    Particle::renderAll();

    renderFilter();

    for (Entity *ignoreFilterEntity: ignoreFilterEntityVector) {
        gWindow->renderEntity(ignoreFilterEntity);
    }
}

void World::renderDebugMode() {
    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

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
