//
// Created by pyrow on 11/02/2024.
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <SDL2/SDL.h>

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

    void showSelf();

    void hideSelf();

    [[nodiscard]] inline Weapon *getWeapon() const { return pWeapon; };

    [[nodiscard]] inline bool isShowingSelf() const { return isShown; }

private:
    Weapon *pWeapon;
    Button *wpButton;
    Text *wpText;

    Artifact *artfArray[ARTIFACT_COUNT];
    Button *artfButtons[ARTIFACT_COUNT];
    Text *artfTexts[ARTIFACT_COUNT];

    bool isShown;
};

#endif
