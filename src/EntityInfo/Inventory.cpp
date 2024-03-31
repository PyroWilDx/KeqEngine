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

Inventory::Inventory()
        : artfArray(), artfButtons(), artfTexts() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    pWeapon = new Weapon(DULL_BLADE);
    wpButton = new Button(16, 16, 96, 96, 6);
    wpButton->changeColor(&Colors::dColorCyan);
    wpButton->setTexture(gWindow->loadTexture("res/gfx/inventory/WDullBlade.png"));
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

    eqStats = new FrameText(860, 16, 360, 96);
    eqStats->addTexts({new Text("first text"),
                       new Text("second text"),
                       new Text("third text")});

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
