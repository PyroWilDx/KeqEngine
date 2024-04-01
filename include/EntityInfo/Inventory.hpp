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

class Equipment;

class Button;

class Text;

class FrameText;

class Inventory {

public:
    Inventory();

    ~Inventory();

    void equipArtifact(int artfType);

    void showSelf();

    void hideSelf();

    void replaceWeapon(Weapon *w);

    Artifact *rerollArtifact(int i);

    double getArtifactsStatValue(int statType);

    inline void setLastSelectedEq(Equipment *lastSelectedEq_) { lastSelectedEq = lastSelectedEq_; }

    inline void setLastClickedButton(Button *lastClickedButton_) { lastClickedButton = lastClickedButton_; }

    [[nodiscard]] inline Weapon *getWeapon() const { return pWeapon; };

    [[nodiscard]] inline Button *getWpButton() const { return wpButton; }

    [[nodiscard]] inline Text *getWpText() const { return wpText; }

    [[nodiscard]] inline Artifact **getArtfArray() const { return (Artifact **) artfArray; }

    [[nodiscard]] inline Equipment *getLastSelectedEq() const { return lastSelectedEq; }

    [[nodiscard]] inline Button *getLastClickedButton() const { return lastClickedButton; }

    [[nodiscard]] inline FrameText *getEqStats() const { return eqStats; }

    [[nodiscard]] inline Button *getOtherButton() const { return otherButton; }

    [[nodiscard]] inline bool isShowingSelf() const { return isShown; }

private:
    Weapon *pWeapon;
    Button *wpButton;
    Text *wpText;

    Artifact *artfArray[ARTIFACT_COUNT];
    Button *artfButtons[ARTIFACT_COUNT];
    Text *artfTexts[ARTIFACT_COUNT];

    Equipment *lastSelectedEq;
    Button *lastClickedButton;

    FrameText *eqStats;
    Button *levelUpButton;
    Button *otherButton;

    bool isShown;
};

#endif
