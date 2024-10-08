//
// Created by pyrowildx on 07/07/2023.
//

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include "AbstractEntity/WorldEntity.hpp"

#define BASE_OUTLINE 6

class Text;

typedef enum {
    BUTTON_IDLE = 0,
    BUTTON_CLICKED
} ButtonState;

class Button : public WorldEntity {

public:
    Button(double x, double y, int renderW, int renderH, bool rmOutline = true);

    Button(double x, double y, int renderW, int renderH, int outlineThickness,
           bool rmOutline = true);

    Button(double x, double y, int renderW, int renderH, int outlineThickness,
           int outlineDarkerCoeff, bool rmOutline = true);

    ~Button() override;

    void translateOutline(double fromX, double fromY, int fromRW, int fromRH);

    void changeColor(SDL_Color *changeColor);

    void addText(const char *textStr, const SDL_Color *textColor, int fontSize);

    void changeText(const char *textStr);

    inline bool shouldTranslate() override { return false; }

    void renderSelf(SDL_Renderer *gRenderer) override;

    virtual void onClick(int mouseX, int mouseY);

    virtual void onClickedMove(int mouseX, int mouseY, bool isMouseOnButton);

    virtual void onClickRelease(int mouseX, int mouseY, bool isMouseOnButton);

    void simClickRelease();

    inline void setOnClick(void (*fOnClick_)(Button *, int, int, void *)) {
        fOnClick = fOnClick_;
    }

    inline void setOnClickedMove(void (*fOnClickedMove_)(Button *, int, int, void *)) {
        fOnClickedMove = fOnClickedMove_;
    }

    inline void setOnClickRelease(void (*fOnClickRelease_)(Button *, int, int, void *)) {
        fOnClickRelease = fOnClickRelease_;
    }

    inline void setOnClickParams(void *onClickParams_) { onClickParams = onClickParams_; }

    inline void setOnClickedMoveParams(void *onClikedMoveParams_) { onClickedMoveParams = onClikedMoveParams_; }

    inline void setOnClickReleaseParams(void *onClickReleaseParams_) { onClickReleaseParams = onClickReleaseParams_; }

    inline void setOnClick(void (*fOnClick_)(Button *, int, int, void *), void *fParams) {
        setOnClick(fOnClick_);
        setOnClickParams(fParams);
    }

    inline void setOnClickedMove(void (*fOnClickedMove_)(Button *, int, int, void *), void *fParams) {
        setOnClickedMove(fOnClickedMove_);
        setOnClickedMoveParams(fParams);
    }

    inline void setOnClickRelease(void (*fOnClickRelease_)(Button *, int, int, void *), void *fParams) {
        setOnClickRelease(fOnClickRelease_);
        setOnClickReleaseParams(fParams);
    }

    inline void setOnDestroy(void (*fOnDestroy_)(Button *, void *), void *fParams) {
        fOnDestroy = fOnDestroy_;
        onDestroyParams = fParams;
    }

    inline void setSwapColorOnClick(bool swapColorOnClick_) { swapColorOnClick = swapColorOnClick_; }

    inline void setState(ButtonState buttonState_) { buttonState = buttonState_; }

    inline void setOutlineThickness(int outlineThickness_) { outlineThickness = outlineThickness_; }

    [[nodiscard]] inline Text *getButtonText() const { return buttonText; }

protected:
    void (*fOnClick)(Button *, int, int, void *);

    void (*fOnClickedMove)(Button *, int, int, void *);

    void (*fOnClickRelease)(Button *, int, int, void *);

    void (*fOnDestroy)(Button *, void  *);

    void *onClickParams;
    void *onClickedMoveParams;
    void *onClickReleaseParams;
    void *onDestroyParams;
    bool swapColorOnClick;
    ButtonState buttonState;
    SDL_Color buttonColor;
    Text *buttonText;
    int outlineThickness;
    int outlineDarkerCoeff;

};

#endif
