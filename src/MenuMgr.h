#ifndef MENU_MGR__H
#define MENU_MGR__H

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "ErrorInfo.h"
#include "FastDelegate.h"

class Languages;
class cLabelLinkGfx;
class cLabelGfx;

class cMenuMgr {
public:
    enum {
        MENU_ROOT,
        MENU_OPTIONS,
        MENU_CREDITS,
        MENU_GAME,
        MENU_HELP,
        QUITAPP
    };
    enum { MYIDLABELURL = 0, MYIDLABELVER = 1 };

    cMenuMgr(LPMenuDelegator pApp);
    virtual ~cMenuMgr();
    LPErrInApp Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer);
    void HandleRootMenu();
    void SetBackground(SDL_Surface* pVal) { m_pScene_background = pVal; }
    void LabelClicked(int iButID);

private:
    void drawBackground();
    void drawStringSH(const char* tmp, int x, int y, SDL_Color& color,
                      TTF_Font* customfont);
    void drawStaticSpriteEx(int src_x, int src_y, int src_dx, int src_dy,
                            int dst_x, int dst_y, SDL_Surface* sprite);
    void drawRect(int x, int y, int dx, int dy, SDL_Color c);
    void drawStaticLine(int x0, int y0, int x1, int y1, SDL_Color color);
    void setPixel(SDL_Surface* pSurface, int x, int y, SDL_Color color);
    void fillRect(int x0, int y0, int width, int height, Uint32 color);
    void rootMenuNext();

private:
    static const SDL_Color staColor_on;
    static const SDL_Color staColor_off;
    static const SDL_Color staColor_white;
    static const SDL_Color staColor_ombre;
    static const SDL_Color staColor_black;
    static const SDL_Color staColor_gray;

private:
    LPMenuDelegator m_pApp;
    Languages* m_pLanString;
    TTF_Font* m_pfont1;
    TTF_Font* m_pfont2;
    SDL_Surface* m_pScreen;
    SDL_Renderer* m_psdlRenderer;
    int m_ifocus_valuesM_A;
    int m_iDeby;
    int m_iDebx;
    int m_iSx;
    int m_iSy;
    SDL_Surface* m_pMenuBox;
    SDL_Surface* m_pScene_background;
    SDL_Rect m_rctPanel;
    cLabelGfx* m_pLabelVersion;
    cLabelLinkGfx* m_phomeUrl;
    TTF_Font* m_pfont3;
    bool m_bMouseInside;
};

#endif
