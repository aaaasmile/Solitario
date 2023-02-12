#ifndef _CBUTTON_GFX_H
#define _CBUTTON_GFX_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class ButtonGfx {
public:
    enum VisbleState { VISIBLE, INVISIBLE, DISABLED };
    enum ButtonType { TEXT_BUTTON, BITMAP_BUTTON };
    enum MouseState { OUTSIDE, INSIDE };

    ButtonGfx();
    ~ButtonGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, ClickCb& fncbClickEvent);
    void SetButtonText(LPCSTR text) {
        _buttonText = text;
        _buttonType = ButtonType::TEXT_BUTTON;
    }
    bool MouseMove(SDL_Event& event);
    bool MouseUp(SDL_Event& event);
    bool MouseDown(SDL_Event& event);
    void DrawButton(SDL_Surface* pScreen);
    void Enable(bool bVal) { _enabled = bVal; }
    void SetVisibleState(VisbleState eVal) { _visibleState = eVal; }
    void SetBitmapSurfaces(SDL_Surface* pS1, SDL_Surface* pS2,
                           SDL_Surface* pS3) {
        _p_bitmap_Act1 = pS1;
        _p_bitmap_MOv2 = pS2;
        _p_bitmap_Pus3 = pS3;
        _buttonType = ButtonType::BITMAP_BUTTON;
    }

private:
    void drawBtAsBitmap(SDL_Surface* pScreen);

private:
    SDL_Surface* _p_buttonSurface;
    TTF_Font* _p_fontText;

    int _butID;
    bool _mouseIsDown;
    ButtonType _buttonType;
    SDL_Surface* _p_bitmap_Act1;
    SDL_Surface* _p_bitmap_MOv2;
    SDL_Surface* _p_bitmap_Pus3;
    MouseState _mouseState;
    VisbleState _visibleState;
    SDL_Rect _rctButton;
    STRING _buttonText;
    bool _enabled;
    ClickCb _fncbClickEvent;
};

#endif
