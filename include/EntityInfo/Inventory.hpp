//
// Created by pyrow on 11/02/2024.
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>

class Weapon;

class Artifact;

class Inventory {

public:
    Inventory();

    ~Inventory();

    [[nodiscard]] inline Weapon *getWeapon() const { return pWeapon; };

private:
    Weapon *pWeapon;
    std::vector<Artifact *> artfVector;


};

#endif
