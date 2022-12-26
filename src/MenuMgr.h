#ifndef MENU_MGR__H
#define MENU_MGR__H

#if _MSC_VER > 1000
#pragma once
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "ErrorInfo.h"
#include "Traits.h"

class Languages;
class cLabelLinkGfx;
class cLabelGfx;

using namespace traits;

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

    cMenuMgr();
    virtual ~cMenuMgr();
    LPErrInApp Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer,
                          MenuDelegator& pApp);
    LPErrInApp HandleRootMenu();
    void SetBackground(SDL_Surface* pVal) { _p_Scene_background = pVal; }
    void LabelClicked(int iButID);

private:
    void drawBackground();
    LPErrInApp drawStringSH(const char* tmp, int x, int y, SDL_Color& color,
                            TTF_Font* customfont);
    void drawStaticSpriteEx(int src_x, int src_y, int src_dx, int src_dy,
                            int dst_x, int dst_y, SDL_Surface* sprite);
    void drawRect(int x, int y, int dx, int dy, SDL_Color c);
    void drawStaticLine(int x0, int y0, int x1, int y1, SDL_Color color);
    void setPixel(SDL_Surface* pSurface, int x, int y, SDL_Color color);
    void fillRect(int x0, int y0, int width, int height, Uint32 color);
    void rootMenuNext();
    ClickCb prepClickCb();

private:
    static const SDL_Color staColor_on;
    static const SDL_Color staColor_off;
    static const SDL_Color staColor_white;
    static const SDL_Color staColor_ombre;
    static const SDL_Color staColor_black;
    static const SDL_Color staColor_gray;

private:
    MenuDelegator _menuDlgt;
    Languages* _p_Languages;
    TTF_Font* _p_font1;
    TTF_Font* _p_font2;
    TTF_Font* _p_font3;
    SDL_Surface* _p_Screen;
    SDL_Renderer* _p_sdlRenderer;
    int _ifocus_valuesM_A;
    int _iDeby;
    int _iDebx;
    int _iSx;
    int _iSy;
    SDL_Surface* _p_MenuBox;
    SDL_Surface* _p_Scene_background;
    SDL_Rect _rctPanel;
    cLabelGfx* _p_LabelVersion;
    cLabelLinkGfx* _p_homeUrl;
    bool _bMouseInside;
};

#endif
