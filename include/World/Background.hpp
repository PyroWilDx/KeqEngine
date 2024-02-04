//
// Created by pyrowildx on 01/06/23.
//

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "AbstractEntity/WorldEntity.hpp"

#define LERP_T 0.056

class Background : public WorldEntity {

public:
    Background(int renderW, int renderH, int totalWidth, int totalHeight,
               const char *imgPath);

    ~Background() override = default;

    void fitImgToScreen();

    bool shouldTranslate() override;

    void fitScreen();

    void translateTo(int x, int y);

    void hardTranslate(Entity *entity);

    void lerpTranslate(Entity *entity);

    void shakeScreen(int shakeForceX, int shakeForceY, int sDuration);

    void doShake();

    void renderSelf(SDL_Renderer *gRenderer) override;

    [[nodiscard]] inline int getTotalW() const { return totalWidth; }

    [[nodiscard]] inline int getTotalH() const { return totalHeight; }

private:
    int totalWidth;
    int totalHeight;
    double preciseFrameX;
    double preciseFrameY;

    int baseFrameW;
    int baseFrameH;

    int baseShakeX, baseShakeY;
    int lastShakeX, lastShakeY;
    int shakeDuration;
    int shakeDurationLeft;
};

#endif
