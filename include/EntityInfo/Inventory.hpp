//
// Created by pyrow on 11/02/2024.
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>

#define ARTIFACT_COUNT 5

class Weapon;

class Artifact;

class Button;

class Text;

class Inventory {

public:
    Inventory();

    ~Inventory();

    void equipArtifact(int artfType);

    [[nodiscard]] inline Weapon *getWeapon() const { return pWeapon; };

private:
    Weapon *pWeapon;
    Button *wpButton;
    Text *wpText;

    Artifact *artfArray[ARTIFACT_COUNT];
    Button *artfButtons[ARTIFACT_COUNT];
    Text *artfTexts[ARTIFACT_COUNT];

};

#endif
