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

Inventory::Inventory()
        : artfArray(), artfButtons(), artfTexts() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    pWeapon = new Weapon(DULL_BLADE);
    wpButton = new Button(16, 16, 96, 96, 6);
    wpButton->changeColor(&Colors::dColorCyan);
    wpButton->setTexture(gWindow->loadTexture("res/gfx/inventory/WDullBlade.png"));
    wpButton->setOnClick([](Button *, int, int, void *) {
        Inventory *kqInventory = Keqing::getInstance()->getInventory();
        Weapon *sword = kqInventory->getWeapon();
        FrameText *frameText = kqInventory->getEqStats();
        char cName[32], cLvl[32], cFlatAtk[32], cAtk[32], cCritRate[32], cCritDmg[32], cElDmg[32];
        sprintf(cName, "Name : %s", sword->getName().c_str());
        sprintf(cLvl, "Level : %d", sword->getLevel());
        sprintf(cFlatAtk, "Flat Attack : %d", sword->getWAtkFlat());
        sprintf(cAtk, "Attack : %.2f%%", sword->getWAtkMultiplier() * 100.);
        sprintf(cCritRate, "Crit Rate : %.2f%%", sword->getWCritRate() * 100.);
        sprintf(cCritDmg, "Crit Damage : %.2f%%", sword->getWCritDamage() * 100.);
        sprintf(cElDmg, "Elemental Damage : %.2f%%", sword->getWElMultiplier() * 100.);
        frameText->changeTexts({
                                       new Text(cName),
                                       new Text(cLvl),
                                       new Text(cFlatAtk),
                                       new Text(cAtk),
                                       new Text(cCritRate),
                                       new Text(cCritDmg),
                                       new Text(cElDmg)
                               });
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

    eqStats = new FrameText(860, 16, 360, 256);
    eqStats->addTexts({new Text("Click on an equipment to display info.")});

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
    artfButtons[artfType]->setOnClick([](Button *, int, int, void *fParams) {
        auto *artf = (Artifact *) fParams;
        FrameText *frameText = Keqing::getInstance()->getInventory()->getEqStats();
        char cName[32], cLvl[32], cMain[32], cSub1[32], cSub2[32], cSub3[32], cSub4[32];

        std::string artfName;
        Artifact::getArtifactInfo(artf->getArtfType(), nullptr, &artfName);
        sprintf(cName, "Name : %s", artfName.c_str());

        sprintf(cLvl, "Level : %d", artf->getLevel());

        StatInfo *mainStat = artf->getStatInfo(0);
        sprintf(cMain, "Main - %s : %.2f%c", Artifact::getStatName(mainStat->statType),
                Artifact::isStatFlatName(mainStat->statType) ? mainStat->statValue : mainStat->statValue * 100.,
                Artifact::isStatFlatName(mainStat->statType) ? '\0' : '%');

        StatInfo *subStat1 = artf->getStatInfo(1);
        sprintf(cSub1, "Sub1 - %s : %.2f%c", Artifact::getStatName(subStat1->statType),
                Artifact::isStatFlatName(subStat1->statType) ? subStat1->statValue : subStat1->statValue * 100.,
                Artifact::isStatFlatName(subStat1->statType) ? '\0' : '%');

        StatInfo *subStat2 = artf->getStatInfo(2);
        sprintf(cSub2, "Sub2 - %s : %.2f%c", Artifact::getStatName(subStat2->statType),
                Artifact::isStatFlatName(subStat2->statType) ? subStat2->statValue : subStat2->statValue * 100.,
                Artifact::isStatFlatName(subStat2->statType) ? '\0' : '%');

        StatInfo *subStat3 = artf->getStatInfo(3);
        sprintf(cSub3, "Sub3 - %s : %.2f%c", Artifact::getStatName(subStat3->statType),
                Artifact::isStatFlatName(subStat3->statType) ? subStat3->statValue : subStat3->statValue * 100.,
                Artifact::isStatFlatName(subStat3->statType) ? '\0' : '%');

        StatInfo *subStat4 = artf->getStatInfo(4);
        sprintf(cSub4, "Sub4 - %s : %.2f%c", Artifact::getStatName(subStat4->statType),
                Artifact::isStatFlatName(subStat4->statType) ? subStat4->statValue : subStat4->statValue * 100.,
                Artifact::isStatFlatName(subStat4->statType) ? '\0' : '%');

        frameText->changeTexts({
                                       new Text(cName),
                                       new Text(cLvl),
                                       new Text(cMain),
                                       new Text(cSub1),
                                       new Text(cSub2),
                                       new Text(cSub3),
                                       new Text(cSub4)
                               });
    }, (void *) artfArray[artfType]);

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

    isShown = false;
}
