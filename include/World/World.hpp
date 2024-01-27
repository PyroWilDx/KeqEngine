//
// Created by pyrowildx on 25/06/2023.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_map>
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
class Keqing;
class Attack;

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

    void refreshPixelsOnRemove(WorldEntity *worldEntity);

    bool xyOutOfBounds(double x, double y);

    Pixel getPixel(double x, double y);

    bool isPixelCode(double x, double y, int worldCode);

    void addButton(Button *button);

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

    void addOtherEntity(Entity *otherEntity);

    void addMenuEntity(Entity *menuEntity);

    void addKQAtk(Attack *atk);

    Attack *addKQAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                     int damage, double kbXVelocity, double kbYVelocity);

    Attack *addKQAtk(LivingEntity *atkIssuer, Entity *followEntity,
                     double xyArray[][2], int arrayLength,
                     int damage, double kbXVelocity, double kbYVelocity);

    void addMonsterAtk(Attack *atk);

    Attack *addMonsterAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                          int damage, double kbXVelocity, double kbYVelocity);

    Attack *addMonsterAtk(LivingEntity *atkIssuer, Entity *followEntity,
                          double xyArray[][2], int arrayLength,
                          int damage, double kbXVelocity, double kbYVelocity);

    void enableColorFilter(Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                           double speed = 0.16);

    void disableColorFilter();

    void addIgnoreFilterEntity(Entity *ignoreFilterEntity);

    void onGameFrame();

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

    inline Background *getBackground() { return background; }

private:
    void addWorldEntity(WorldEntity *worldEntity);

    Background *background;
    Entity *translateBackgroundEntity;
    std::unordered_map<int, Button *> buttonHashMap;
    Button *activeButton;
    std::vector<Block *> blockVector;
    std::vector<Monster *> monsterVector;
    std::vector<Entity *> otherEntityVecotr;
    bool renderKeqing;
    LinkedList *kqAtkLL;
    LinkedList *monsterAtkLL;
    Filter colorFilter;
    double colorFilterSpeed;
    std::vector<Entity *> ignoreFilterEntityVector;
    bool displayMenu;
    std::vector<Entity *> menuEntityVector;
    Pixel **pixels;
    int stopSoundOnQuit;

    void (*onQuit)();

};

#endif
