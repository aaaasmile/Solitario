
#if !defined(AFX_CMENUMGR_H__BE9C3990_5883_489E_943E_3D9832DBC369__INCLUDED_)
#define AFX_CMENUMGR_H__BE9C3990_5883_489E_943E_3D9832DBC369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

class AppGfx;
class SolitarioGfx;
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

    cMenuMgr(AppGfx* pApp, SolitarioGfx* pGfx);
    virtual ~cMenuMgr();
    void Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer);
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
    AppGfx* m_pApp;
    Languages* m_pLanString;
    TTF_Font* m_pfont1;
    TTF_Font* m_pfont2;
    SolitarioGfx* m_pInvidoGfx;
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
