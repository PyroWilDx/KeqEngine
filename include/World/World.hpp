//
// Created by pyrowildx on 25/06/2023.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_map>
#include <list>
#include <SDL2/SDL.h>
#include "Utils/LinkedList.hpp"

#define GET_NEAREST_WALL_RETURN_NONE (-42)

class Background;

class Button;

class Block;

class Entity;

class WorldEntity;

class LivingEntity;

class Monster;

class DamageText;

class Keqing;

class Attack;

class ToastText;

typedef struct Filter {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    double currAlpha;
    double targetAlpha;
    bool filterActivated;
} Filter;

typedef struct Pixel {
    int worldType;
    int worldCode;
} Pixel;

enum {
    STOP_SOUND_NONE = 0,
    STOP_SOUND_CHUNK,
    STOP_SOUND_MUSIC,
    STOP_SOUND_ALL
};

class World {

public:
    World(int screenW, int screenH,
          int backgroundTotalW, int backgroundTotalH,
          const char *backgroundImgPath);

    ~World();

    void updatePixels(int x1, int y1, int x2, int y2, WorldEntity *worldEntity);

    void updatePixels(SDL_Rect *rect, WorldEntity *worldEntity);

    void refreshPixelsOnMove(WorldEntity *worldEntity, SDL_Rect *lastRect);

    void refreshPixelsOnRemove(WorldEntity *worldEntity);

    bool xyOutOfBounds(double x, double y);

    Pixel getPixel(double x, double y);

    bool isPixelCode(double x, double y, int worldCode);

    void addButton(Button *button);

    void removeButtonNoFree(Button *button);

    void removeButton(Button **button);

    bool isPixelButton(double x, double y);

    void clickPixel(double x, double y, Uint32 eventType);

    void addBlock(Block *block);

    Block *addBlock(int blockCode, double x, double y, int renderW, int renderH);

    Block *addBlock(int blockCode, double x, double y, int renderW);

    void addCoveredBlock(int blockCode, int coverBlockCode, double x, double y,
                         int renderW);

    void removeBlock(Block **block);

    bool isPixelBlock(double x, double y);

    bool isPixelSurface(double x, double y);

    int getNearestWallFrom(double x, double y, int direction);

    void addMonster(Monster *monster);

    void removeMonster(Monster *monster);

    void addOtherEntity(Entity *otherEntity);

    void removeOtherEntity(Entity *otherEntity);

    void showToast(ToastText *toastText);

    void removeToast();

    void addKQAtk(Attack *atk, double atkPercent);

    Attack *addKQAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                     double atkPercent, double kbXVelocity, double kbYVelocity);

    Attack *addKQAtk(LivingEntity *atkIssuer, Entity *followEntity,
                     double xyArray[][2], int arrayLength,
                     double atkPercent, double kbXVelocity, double kbYVelocity);

    void addMonsterAtk(Attack *atk);

    Attack *addMonsterAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                          int damage, double kbXVelocity, double kbYVelocity);

    Attack *addMonsterAtk(LivingEntity *atkIssuer, Entity *followEntity,
                          double xyArray[][2], int arrayLength,
                          int damage, double kbXVelocity, double kbYVelocity);

    void enableColorFilter(Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                           double speed = 0.16, bool restart = true);

    void disableColorFilter();

    void addIgnoreFilterEntity(Entity *ignoreFilterEntity);

    void pushBackDamageText(DamageText *dmgText);

    void popFrontDamageText();

    void addMenuEntity(Entity *menuEntity);

    void removeMenuEntityNoFree(Entity *menuEntity);

    void removeMenuEntity(Entity *menuEntity);

    bool isDisplayingMenu();

    void onGameFrame();

    void onGameFrameMenu();

    void renderFilter();

    void renderSelf();

    void renderDebugMode();

    void onEntityRemove(Entity *removedEntity);

    inline void setTranslateEntity(Entity *translateEntity) { translateBackgroundEntity = translateEntity; }

    inline void setRenderKeqing(bool renderKeqing_) { renderKeqing = renderKeqing_; }

    inline void setDisplayMenu(bool displayMenu_) {
        displayMenu = displayMenu_;
    }

    inline void setOnQuit(void (*onQuit_)()) { onQuit = onQuit_; }

    [[nodiscard]] inline Background *getBackground() { return background; }

private:
    void addWorldEntity(WorldEntity *worldEntity);

    Background *background;
    Entity *translateBackgroundEntity;
    std::unordered_map<int, Button *> buttonHashMap;
    Button *activeButton;
    std::vector<Block *> blockVector;
    std::vector<Monster *> monsterVector;
    std::vector<Entity *> otherEntityVector;
    bool renderKeqing;
    LinkedList *kqAtkLL;
    LinkedList *monsterAtkLL;
    Filter colorFilter;
    double colorFilterSpeed;
    std::vector<Entity *> ignoreFilterEntityVector;
    std::list<DamageText *> dmgTextLL;
    bool displayMenu;
    std::vector<Entity *> menuEntityVector;
    ToastText *currToastText;
    Pixel **pixels;
    int stopSoundOnQuit;

    void (*onQuit)();

};

#endif
