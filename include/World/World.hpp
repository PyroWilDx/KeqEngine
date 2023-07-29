//
// Created by pyrowildx on 25/06/2023.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_map>
#include "Background.hpp"
#include "UI/Button.hpp"
#include "Block.hpp"
#include "EntityBase/Monster.hpp"
#include "Utils/LinkedList.hpp"
#include "Attack.hpp"

typedef struct Pixel {
    int worldType;
    int worldCode;
} Pixel;

class World {

public:
    World(int screenW, int screenH,
          int backgroundTotalW, int backgroundTotalH,
          const char *backgroundImgPath);

    ~World();

    void updatePixels(int x1, int y1, int x2, int y2, WorldEntity *worldEntity);

    void addBlock(Block *block);

    void addBlock(int blockCode, double x, double y, int renderW, int renderH);

    void addButton(Button *button);

    void addMonster(Monster *monster);

    void addKQAtk(Attack *atk);

    Attack *addKQAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                     int damage, double kbXVelocity, double kbYVelocity);

    void addMonsterAtk(Attack *atk);

    Attack *addMonsterAtk(LivingEntity *atkIssuer, double xyArray[][2], int arrayLength,
                       int damage, double kbXVelocity, double kbYVelocity);

    Pixel getPixel(double x, double y);

    bool isPixelBlock(double x, double y);

    bool isPixelButton(double x, double y);

    bool isPixelSurface(double x, double y);

    bool isPixelCode(double x, double y, int worldCode);

    double getNearestWallFrom(double x, double y, int direction);

    void clickPixel(double x, double y, Uint32 eventType);

    void onGameFrame();

    void renderSelf();

    void renderDebugMode();

    inline Background *getBackground() { return background; }

private:
    void addWorldEntity(WorldEntity *worldEntity);

    Background *background;
    std::vector<Block *> blockVector;
    std::unordered_map<int, Button *> buttonHashMap;
    Button *activeButton;
    std::vector<Monster *> monsterVector;
    LinkedList *kqAtkLL;
    LinkedList *monsterAtkLL;
    Pixel **pixels;

};

#endif
