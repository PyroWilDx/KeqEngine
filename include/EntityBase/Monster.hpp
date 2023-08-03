//
// Created by pyrow on 24/07/2023.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "LivingEntity.hpp"

class Monster : public LivingEntity {

    using LivingEntity::LivingEntity;

public:
    Monster() : LivingEntity(0, 0, 0, 0, 0) {}

    virtual void AI() = 0;

private:

};

#endif
