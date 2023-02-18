#ifndef CLABELLINKGFX__H
#define CLABELLINKGFX__H

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class LabelLinkGfx {
public:
    enum eSate { VISIBLE, INVISIBLE, PRESSED };

    LabelLinkGfx();
    virtual ~LabelLinkGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, ClickCb& fncbClickEvent);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    void MouseMove(SDL_Event& event);
    void MouseUp(SDL_Event& event);
    virtual void Draw(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { m_bIsEnabled = bVal; }
    void SetState(eSate eVal);
    void SetUrl(LPCSTR lpszUrl) { m_strUrl = lpszUrl; }

public:
    eSate m_eState;
    SDL_Rect m_rctButt;
    STRING m_strButText;
    bool m_bIsEnabled;
    ClickCb m_fncbClickEvent;

protected:
    SDL_Surface* m_pSurf_Bar;
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    int m_iButID;
    STRING m_strUrl;
    bool _isDirty;
    bool _mouseOuside;
};

#endif
