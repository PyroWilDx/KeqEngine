//
// Created by pyrowildx on 24/07/2023.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "LivingEntity.hpp"

class Monster : public LivingEntity {

    using LivingEntity::LivingEntity;

public:
    virtual void AI() = 0;

private:

};

#endif
