//
// Created by pyrow on 11/02/2024.
//

#include "EntityInfo/Inventory.hpp"

Inventory::Inventory() {
    weapon
}

Inventory::~Inventory() {
    delete weapon;

    for (Artifact *artifact: artifactVector) {
        delete artifact;
    }
}
