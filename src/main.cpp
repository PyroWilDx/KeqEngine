#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Keqing.hpp"
#include "Text.hpp"
#include "Monster.hpp"
#include "Background.hpp"
#include "MonsterLinkedList.hpp"
#include "Particle.hpp"

// Text constants
const char *fontPath = "res/fonts/JetBrainsMono-Regular.ttf";
const SDL_Color FPSTextColor = {255, 255, 255};
const int FPSFontSize = 40;

int main() {
    std::string path = std::filesystem::current_path();
    std::filesystem::current_path(path + "/..");

    myAssert(SDL_Init(SDL_INIT_VIDEO) >= 0, "SDL_Init FAILED.", SDL_GetError());
    myAssert(IMG_Init(IMG_INIT_PNG) != 0, "IMG_Init FAILED.", SDL_GetError());
    myAssert(TTF_Init() != -1, "TTF_Init FAILED.", SDL_GetError());

    WindowRenderer window = WindowRenderer("Keqing", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Texture *backgroundTexture = window.loadTexture("res/gfx/background.png");
    Background background = Background(SCREEN_WIDTH, SCREEN_HEIGHT,
                                       3000, backgroundTexture);

    Particle::initParticle(&window);

    Keqing::initKeqing(&window);
    Keqing *kq = Keqing::getInstance();
    kq->moveTo(0, DEFAULT_Y, 0);
    kq->setRenderWHMultiplier(KQ_WIDTH_MULTIPLIER, KQ_HEIGHT_MULTIPLIER);
    kq->setCollisionRect({(int) (0.0f * KQ_WIDTH_MULTIPLIER),
                          (int) (12.0f * KQ_HEIGHT_MULTIPLIER),
                          (int) (60.0f * KQ_WIDTH_MULTIPLIER),
                          (int) (84.0f * KQ_HEIGHT_MULTIPLIER)});

    Particle **particules;

    auto *monsterLL = new MonsterLinkedList();
//    auto *zombie0 = new Monster(96, 96, &window);
//    zombie0->moveTo(100, DEFAULT_Y, -100);
//    zombie0->setCollisionRect({70, 76, 76, 120});
//    zombie0->setRenderWHMultiplier(MONSTER_WIDTH, MONSTER_HEIGHT);
//    zombie0->walk();
//    monsterLL->insert(zombie0);
//    auto *zombie1 = new Monster(96, 96, &window);
//    zombie1->moveTo(200, DEFAULT_Y, -70);
//    zombie1->setCollisionRect({70, 76, 76, 120});
//    zombie1->setRenderWHMultiplier(MONSTER_WIDTH, MONSTER_HEIGHT);
//    zombie1->run();
//    monsterLL->insert(zombie1);
//    Monster *zombie2 = zombie0->copy(&window);
//    zombie2->moveTo(300, DEFAULT_Y, -50);
//    zombie2->run();
//    monsterLL->insert(zombie2);
//    Monster *zombie3 = zombie0->copy(&window);
//    zombie3->moveTo(400, DEFAULT_Y, -25);
//    zombie3->walk();
//    monsterLL->insert(zombie3);

    Uint32 dt;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;

    Text FPSText = Text();
    Uint32 accumulatedFPSTime = 10000;
    Uint32 accumulatedFrames = 0;

    SDL_Event event;
    int key;
    bool keyPressed[4] = {false, false, false, false};

    bool gameRunning = true;
    while (gameRunning) {

        // Events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    key = event.key.keysym.sym;
                    switch (key) {
                        case SDLK_q:
                        case SDLK_d:
                        case SDLK_s:
                        case SDLK_z:
                            keyPressed[key % 4] = true;
                            kq->updateDirection(key, keyPressed);
                            break;
                        case SDLK_SPACE:
                            kq->setTextureAnimated(KQ_JUMP_SPRITE, true);
                            break;
                        case SDLK_LSHIFT:
                            if (!kq->isDashing())
                                kq->setTextureAnimated(KQ_DASH_START_SPRITE, true);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    key = event.key.keysym.sym;
                    switch (key) {
                        case SDLK_q:
                        case SDLK_d:
                        case SDLK_s:
                        case SDLK_z:
                            keyPressed[key % 4] = false;
                            kq->clearDirection(key, keyPressed);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    kq->setTextureAnimated(KQ_NATTACKS_SPRITE, true);
                    break;
                default:
                    break;
            }
        }

        // Time Handling
        currentTime = SDL_GetTicks();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        // Keqing
        if (kq->isMoving()) { // TODO Need to handle other movements too
            kq->move((int) dt);
            int kqX = kq->getX() + kq->getCollisionRect().x;
            int kqW = kq->getCollisionRect().w;
            int halfX = SCREEN_WIDTH / 2 - kqW / 2;
            int kqXDirection = kq->getXDirection();
            if ((kqXDirection == 1 && kqX > halfX) ||
                (kqXDirection == -1 && kqX < halfX)) {
                int lastX = background.getFrame().x;
                background.move((int) dt, kq->getXDirection());
                int translateX = lastX - background.getFrame().x;
                // Translate all Entities
                kq->addX(translateX);
                monsterLL->operateAllCells(&Monster::addX, &translateX, nullptr);
            }
        }
        if (kq->isJumping()) kq->jump((int) dt);
        if (kq->isNAttacking()) kq->nattack((int) dt, (int) currentTime);
        if (kq->isDashing()) kq->dash((int) dt);
        if (kq->isDamaged()) kq->damage((int) dt);
        kq->animate((int) dt);

        // Monster(s)
        monsterLL->operateAllCells(&Monster::move, &dt, nullptr);
        if (rand() % 1000 < 10) {
            monsterLL->operateAllCells(&Monster::attack, nullptr, nullptr);
        }
        monsterLL->operateAllCells(&Monster::animate, &dt, nullptr);

        // Handling Keqing Collisions
        bool kqDamaged = false;
        monsterLL->operateAllCells(&Monster::collides, nullptr, &kqDamaged);
        if (kqDamaged) {
            kq->damage((int) dt); // TODO change
            printf("Noob %d\n", kq->getHp());
        }

        // Particles
        particules = Particle::getActiveParticles();
        for (int i = 0; i < Particle::getCount(); i++) {
            if (particules[i] == nullptr) continue;

            particules[i]->animate((int) dt);
            if (particules[i]->isFinished()) {
                Particle::remove(i);
            }
        }

        // FPS Text
        if (accumulatedFPSTime > 1000) {
            char text[16];
            sprintf(text, "FPS : %d", accumulatedFrames);
            FPSText.loadTextTexture(0, 0, text, &FPSTextColor,
                                    fontPath, FPSFontSize, window.getRenderer());
            accumulatedFPSTime = 0;
            accumulatedFrames = 0;
        }
        accumulatedFPSTime += dt;
        accumulatedFrames++;

        window.clear();
        window.render(&background);
        window.render(&FPSText);
        monsterLL->operateAllCells(&Monster::render, &window, nullptr);
        window.render(kq);
        for (int i = 0; i < Particle::getCount(); i++) {
            if (particules[i] == nullptr) continue;

            window.renderParticle(particules[i]);
        }
        window.display();
        printf("%d\n", kq->getX());
    }

    // Free
    background.destroy();
    FPSText.destroy();
    monsterLL->deleteAllCells();
    delete monsterLL;
    kq->destroy();
    Particle::cleanUp();
    window.cleanUp();
    SDL_Quit();

    return EXIT_SUCCESS;
}
