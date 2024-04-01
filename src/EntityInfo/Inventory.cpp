//
// Created by pyrow on 11/02/2024.
//

#include "EntityInfo/Inventory.hpp"
#include "EntityInfo/Weapon.hpp"
#include "EntityInfo/Artifact.hpp"
#include "UI/Button.hpp"
#include "Text/Text.hpp"
#include "WindowRenderer.hpp"
#include "World/World.hpp"
#include "Utils/Global.hpp"
#include "Utils/Colors.hpp"
#include "UI/FrameText.hpp"
#include "Keqing.hpp"
#include "UI/ToastText.hpp"

Inventory::Inventory()
        : artfArray(), artfButtons(), artfTexts() {

    pWeapon = new Weapon(DULL_BLADE);
    wpButton = new Button(16, 16, 96, 96, 6);
    wpButton->changeColor(&Colors::dColorCyan);
    wpButton->setTexture(pWeapon->getTextureFromName());
    wpButton->setOnClickRelease([](Button *self, int, int, void *) {
        Inventory *kqInventory = Keqing::getInstance()->getInventory();
        Weapon *sword = kqInventory->getWeapon();

        kqInventory->setLastSelectedEq(sword);
        kqInventory->setLastClickedButton(self);

        const int N = 96;
        char cName[N], cLvl[N], cFlatAtk[N], cAtk[N], cCritRate[N], cCritDmg[N], cElDmg[N];
        sprintf(cName, "Name : %s", sword->getName().c_str());
        sprintf(cLvl, "Level : %d", sword->getLevel());
        sprintf(cFlatAtk, "Flat Attack : %d", sword->getWAtkFlat());
        sprintf(cAtk, "Attack : %.2f%%", sword->getWAtkMultiplier() * 100.);
        sprintf(cCritRate, "Crit Rate : %.2f%%", sword->getWCritRate() * 100.);
        sprintf(cCritDmg, "Crit Damage : %.2f%%", sword->getWCritDamage() * 100.);
        sprintf(cElDmg, "Elemental Damage : %.2f%%", sword->getWElMultiplier() * 100.);
        kqInventory->getEqStats()->changeTexts({
                                                       new Text(cName),
                                                       new Text(cLvl),
                                                       new Text(cFlatAtk),
                                                       new Text(cAtk),
                                                       new Text(cCritRate),
                                                       new Text(cCritDmg),
                                                       new Text(cElDmg)
                                               });

        kqInventory->getOtherButton()->changeText("Upgrade");
    });
    wpText = new Text(16, 120, "Dull Blade",
                      16, false);

    for (int i = 0; i < ARTIFACT_COUNT; i++) {
        artfArray[i] = nullptr;
        artfButtons[i] = nullptr;
        artfTexts[i] = nullptr;
    }

    for (int i = 0; i < ARTIFACT_COUNT; i++) {
        equipArtifact(i);
    }

    lastSelectedEq = nullptr;
    lastClickedButton = nullptr;

    eqStats = new FrameText(860, 16, 360, 256);
    eqStats->addTexts({new Text("Click on an equipment to display info.")});

    levelUpButton = new Button(100, 200, 300, 100);
    levelUpButton->changeColor(&Colors::dColorKq);
    levelUpButton->addText("Level Up", &Colors::dColorWhite, 22);
    levelUpButton->setOnClickRelease([](Button *, int, int, void *) {
        Inventory *kqInventory = Keqing::getInstance()->getInventory();
        Equipment *e = kqInventory->getLastSelectedEq();
        if (e != nullptr) {
            if (e->levelUp()) ToastText::makeToast("Equipment Upgraded !");
            else ToastText::makeToast("Max Level Reached.");
            kqInventory->getLastClickedButton()->simClickRelease();
        } else ToastText::makeToast("No Equipment Selected...");
    });

    otherButton = new Button(460, 200, 300, 100);
    otherButton->changeColor(&Colors::dColorKq);
    otherButton->addText("Upgrade", &Colors::dColorWhite, 22);
    otherButton->setOnClickRelease([](Button *, int, int, void *) {
        Inventory *kqInventory = Keqing::getInstance()->getInventory();
        Equipment *e = kqInventory->getLastSelectedEq();

        if (e == nullptr) {
            ToastText::makeToast("No Equipment Selected...");
            return;
        }

        if (e == kqInventory->getWeapon()) {
            auto *w = (Weapon *) e;
            Weapon *uW = w->getUpgradedWeapon();
            if (uW != nullptr) {
                kqInventory->replaceWeapon(uW);
                kqInventory->getWpButton()->setTexture(uW->getTextureFromName());
                kqInventory->getWpText()->changeText(uW->getName().c_str());
                kqInventory->setLastSelectedEq(uW);
                ToastText::makeToast("Successfuly Upgraded !");
            } else ToastText::makeToast("Can't be Upgraded.");
        } else {
            auto *a = (Artifact *) e;
            Artifact *rA = kqInventory->rerollArtifact(a->getArtfType());
            kqInventory->setLastSelectedEq(rA);
            ToastText::makeToast("Successfuly Rerolled !");
        }
        kqInventory->getLastClickedButton()->simClickRelease();
    });

    isShown = false;
}

Inventory::~Inventory() {
    delete pWeapon;
    if (!isShown) {
        delete wpButton;
        delete wpText;
    }

    for (int i = 0; i < ARTIFACT_COUNT; i++) {
        delete artfArray[i];
        if (!isShown) {
            delete artfButtons[i];
            delete artfTexts[i];
        }
    }

    if (!isShown) delete eqStats;
}

void Inventory::equipArtifact(int artfType) {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    if (artfArray[artfType] != nullptr) delete artfArray[artfType];
    artfArray[artfType] = new Artifact(artfType);

    double itemX = 16 + (artfType + 2) * 112;

    std::string artfImgPath, artfName;
    Artifact::getArtifactInfo(artfType, &artfImgPath, &artfName);

    if (artfButtons[artfType] != nullptr) delete artfButtons[artfType];
    artfButtons[artfType] = new Button(itemX, 16,
                                       96, 96, 6);
    artfButtons[artfType]->changeColor(&Colors::dColorOrange);
    artfButtons[artfType]->setTexture(gWindow->loadTexture(artfImgPath.c_str()));
    artfButtons[artfType]->setOnClickRelease([](Button *self, int, int, void *fParams) {
        auto *artf = *(Artifact **) fParams;
        std::string artfName;
        Artifact::getArtifactInfo(artf->getArtfType(), nullptr, &artfName);
        Inventory *kqInventory = Keqing::getInstance()->getInventory();

        kqInventory->setLastSelectedEq(artf);
        kqInventory->setLastClickedButton(self);

        const int N = 96;
        char cName[N], cLvl[N], cMain[N], cSub1[N], cSub2[N], cSub3[N], cSub4[N];
        sprintf(cName, "Name : %s", artfName.c_str());
        sprintf(cLvl, "Level : %d", artf->getLevel());
        artf->sprintfStat(cMain, "Main - %s : %.2f%c", 0);
        artf->sprintfStat(cSub1, "Sub1 - %s : %.2f%c", 1);
        artf->sprintfStat(cSub2, "Sub2 - %s : %.2f%c", 2);
        artf->sprintfStat(cSub3, "Sub3 - %s : %.2f%c", 3);
        artf->sprintfStat(cSub4, "Sub4 - %s : %.2f%c", 4);
        kqInventory->getEqStats()->changeTexts({
                                                       new Text(cName),
                                                       new Text(cLvl),
                                                       new Text(cMain),
                                                       new Text(cSub1),
                                                       new Text(cSub2),
                                                       new Text(cSub3),
                                                       new Text(cSub4)
                                               });

        kqInventory->getOtherButton()->changeText("Reroll");
    }, (void *) (artfArray + artfType));

    if (artfTexts[artfType] != nullptr) delete artfTexts[artfType];
    artfTexts[artfType] = new Text(itemX, 120, artfName.c_str(),
                                   16, false);
}

void Inventory::showSelf() {
    World *gWorld = Global::gWorld;

    gWorld->setDisplayMenu(true);
    gWorld->enableColorFilter(156, 156, 156, 96, 0.8, false);

    gWorld->addButton(wpButton);
    gWorld->addMenuEntity(wpText);

    for (int i = 0; i < ARTIFACT_COUNT; i++) {
        if (artfButtons[i] != nullptr) gWorld->addButton(artfButtons[i]);
        if (artfTexts[i] != nullptr) gWorld->addMenuEntity(artfTexts[i]);
    }

    gWorld->addMenuEntity(eqStats);
    gWorld->addButton(levelUpButton);
    gWorld->addButton(otherButton);

    wpButton->simClickRelease();

    isShown = true;
}

void Inventory::hideSelf() {
    World *gWorld = Global::gWorld;

    gWorld->setDisplayMenu(false);
    gWorld->disableColorFilter();

    gWorld->removeButtonNoFree(wpButton);
    gWorld->removeMenuEntityNoFree(wpText);

    for (int i = 0; i < ARTIFACT_COUNT; i++) {
        if (artfButtons[i] != nullptr) gWorld->removeButtonNoFree(artfButtons[i]);
        if (artfTexts[i] != nullptr) gWorld->removeMenuEntityNoFree(artfTexts[i]);
    }

    gWorld->removeMenuEntityNoFree(eqStats);
    gWorld->removeButtonNoFree(levelUpButton);
    gWorld->removeButtonNoFree(otherButton);

    isShown = false;
}

void Inventory::replaceWeapon(Weapon *w) {
    delete pWeapon;
    pWeapon = w;
}

Artifact *Inventory::rerollArtifact(int i) {
    delete artfArray[i];
    artfArray[i] = new Artifact(i);
    return artfArray[i];
}

double Inventory::getArtifactsStatValue(int statType) {
    double value = 0;
    for (Artifact *a : artfArray) {
        value += a->getStatValue(statType);
    }
    return value;
}
