
#ifndef CLABELGFX__H_
#define CLABELGFX__H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "ErrorInfo.h"
#include "WinTypeGlobal.h"

class LabelGfx {
public:
    enum SateGfx { VISIBLE, INVISIBLE, PRESSED };

    LabelGfx();
    virtual ~LabelGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    SDL_Renderer* psdlRenderer);
    void SetWindowText(LPCSTR strCaption) { _CtrlText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Texture* pScene_background, SDL_Texture* pScreenTexture);
    void MouseUp(SDL_Event& event);
    void Draw(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { _isEnabled = bVal; }
    void SetState(SateGfx eVal);
    void Redraw(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                SDL_Texture* pScreenTexture);

private:
    SateGfx _StateGfx;
    SDL_Rect _rctCtrl;
    STRING _CtrlText;
    bool _isEnabled;
    SDL_Renderer* _p_sdlRenderer;
    SDL_Surface* _p_Surf_Bar;
    TTF_Font* _p_FontText;
    SDL_Color _color;
};

#endif
