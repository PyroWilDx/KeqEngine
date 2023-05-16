//
// Created by pyrowildx on 13/05/23.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "WindowRenderer.hpp"
#include "Utils.hpp"

typedef struct SpriteTexture {
    bool animated;
    SDL_Texture *texture;
    int totalWidth;
    int timeBetweenFrames;
} SpriteTexture;

enum {
    IDLE_SPRITE = 0,
    RUN_SPRITE = 1,
    JUMP_SPRITE = 2,
    ATTACK_SPRITE = 3,
    HURT_SPRITE = 4,
    TURN_SPRITE = 5,
    END_SPRITE_ENUM
};

const int PLAYER_WIDTH = 160;
const int PLAYER_HEIGHT = 160;
const int PLAYER_DEFAULT_Y = SCREEN_HEIGHT - PLAYER_HEIGHT;
const int PLAYER_MIN_Y = PLAYER_DEFAULT_Y - 100;
const float PLAYER_SPEED = 0.4;

class Player : public Entity {

public:
    Player(int x, int y, int z, int w, int h, WindowRenderer window);

    void setTextureAnimated(int code, bool animated);

    void updateTexture();

    void updateDirection(int key, const bool *keyPressed);

    void clearDirection(int key, const bool *keyPressed);

    void move(int dt) override;

    void jump(int dt);

    void animate(Uint32 *accumulatedAnimationTime);

    void destroy() override;

    inline bool isMoving() { return (currentSprite == RUN_SPRITE || isJumping()); }

    inline bool isJumping() { return (currentSprite == JUMP_SPRITE); }

private:
    int hp;
    float jumpVelocity;
    SpriteTexture spriteArray[6];
    int currentSprite;
};

#endif
