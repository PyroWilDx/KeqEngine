//
// Created by pyrow on 11/02/2024.
//

#include "EntityInfo/Inventory.hpp"
#include "EntityInfo/Weapon.hpp"
#include "EntityInfo/Artifact.hpp"

Inventory::Inventory() {
    pWeapon = new Weapon(DULL_BLADE);
    artfVector = std::vector<Artifact *>();
}

Inventory::~Inventory() {
    delete pWeapon;

    for (Artifact *artifact: artfVector) {
        delete artifact;
    }
}
