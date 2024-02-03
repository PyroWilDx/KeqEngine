//
// Created by pyrowildx on 24/07/2023.
//

#ifndef SLIME_HPP
#define SLIME_HPP

#include <string>
#include "AbstractEntity/Monster.hpp"

#define SLIME_MAX_DIST 600.
#define SLIME_ATK_DIST 90.

#define SLIME_JUMP_CD 300
#define SLIME_ATK_CD 600

enum {
    SLIME_IDLE,
    SLIME_WALK,
    SLIME_CROUCH,
    SLIME_JUMP,
    SLIME_ATK,
    SLIME_DEATH,
    SLIME_ENUM_N
};

class Slime : public Monster {

public:
    explicit Slime(const std::string &colorString);

    void jump();

    void attack();

    void AI() override;

    void hurt() override;

    void updateAction() override;

private:
    int lastJumpTime;
    int lastAtkTime;

};

#endif
