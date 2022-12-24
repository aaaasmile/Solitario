#ifndef CLABELLINKGFX__H
#define CLABELLINKGFX__H

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "FastDelegate.h"
#include "win_type_global.h"

class cLabelLinkGfx {
    // use fastdelegate to implement the observer pattern and avoid using the
    // callback interface on observer
    // typedef FastDelegate1<int> CLICKEVENT; //TODO delegate

public:
    enum eSate { VISIBLE, INVISIBLE, PRESSED };

    cLabelLinkGfx();
    virtual ~cLabelLinkGfx();

    void Init(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
              int iButID, SDL_Renderer* psdlRenderer);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Texture* pScene_background, SDL_Texture* pScreenTexture);
    void MouseUp(SDL_Event& event);
    virtual void Draw(SDL_Surface* pScreen);
    void EnableWindow(BOOL bVal) { m_bIsEnabled = bVal; }
    void SetState(eSate eVal);
    void Redraw(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                SDL_Texture* pScreenTexture);
    void SetUrl(LPCSTR lpszUrl) { m_strUrl = lpszUrl; }

public:
    eSate m_eState;
    SDL_Rect m_rctButt;
    STRING m_strButText;
    BOOL m_bIsEnabled;
    fastdelegate::LPCLICKEVENT m_fncbClickEvent;

protected:
    SDL_Surface* m_pSurf_Bar;
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    int m_iButID;
    STRING m_strUrl;
    SDL_Renderer* m_psdlRenderer;
};

#endif