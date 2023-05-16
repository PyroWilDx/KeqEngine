//
// Created by pyrowildx on 13/05/23.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "WindowRenderer.hpp"

typedef struct SpriteTexture {
    bool animated;
    SDL_Texture *texture;
    int totalWidth;
    int timeBetweenFrames;
} SpriteTexture;

class Player : public Entity {

public:
    Player(int x, int y, int z, int w, int h, WindowRenderer window);

    void updateDirection(int key, const bool *keyPressed);

    void clearDirection(int key, const bool *keyPressed);

    void jump();

    void updateTexture();

    void animate(Uint32 *accumulatedAnimationTime);

    void destroy() override;

    inline bool isMoving() { return runSprite.animated; }

private:
    int hp;
    SpriteTexture currentSprite;
    SpriteTexture idleSprite;
    SpriteTexture runSprite;
    SpriteTexture jumpSprite;
    SpriteTexture attackSprite;
    SpriteTexture hurtSprite;
};

#endif
