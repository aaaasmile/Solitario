
#ifndef CLABELGFX__H_
#define CLABELGFX__H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "../Traits.h"
#include "ErrorInfo.h"
#include "WinTypeGlobal.h"

using namespace traits;

class LabelGfx {
public:
    enum eSate { VISIBLE, INVISIBLE, PRESSED };

    LabelGfx();
    virtual ~LabelGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, SDL_Renderer* psdlRenderer,
                    ClickCb& fncbClickEvent);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Texture* pScene_background, SDL_Texture* pScreenTexture);
    void MouseUp(SDL_Event& event);
    void Draw(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { m_bIsEnabled = bVal; }
    void SetState(eSate eVal);
    void Redraw(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                SDL_Texture* pScreenTexture);

public:
    eSate m_eState;
    SDL_Rect m_rctButt;
    STRING m_strButText;
    bool m_bIsEnabled;
    ClickCb m_fncbClickEvent;

private:
    SDL_Renderer* m_psdlRenderer;
    SDL_Surface* m_pSurf_Bar;
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    int m_iButID;
};

#endif
