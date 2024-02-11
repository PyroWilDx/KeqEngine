//
// Created by pyrow on 11/02/2024.
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include "Weapon.hpp"
#include "Artifact.hpp"

class Inventory {

public:
    Inventory();

    ~Inventory();

private:
    Weapon *weapon;
    std::vector<Artifact *> artifactVector;


};

#endif
