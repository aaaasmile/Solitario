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
class LabelLinkGfx;
class LabelGfx;

using namespace traits;

class MenuMgr {
public:
    enum {
        MENU_ROOT,
        MENU_OPTIONS,
        MENU_CREDITS,
        MENU_GAME,
        MENU_HELP,
        QUITAPP
    };
    enum { MYIDLABELURL = 0 };

    MenuMgr();
    virtual ~MenuMgr();
    LPErrInApp Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer,
                          MenuDelegator& pmenuDelegator);
    LPErrInApp HandleRootMenu();
    void SetBackground(SDL_Surface* pVal) { _p_SceneBackground = pVal; }

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
    void updateTextureAsFlipScreen();

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
    TTF_Font* _p_fontAriblk;
    TTF_Font* _p_fontVera;
    TTF_Font* _p_fontVeraUnderscore;
    SDL_Surface* _p_Screen;
    SDL_Surface* _p_ScreenBackbuffer;
    SDL_Renderer* _p_sdlRenderer;
    int _focusedMenuItem;
    int _box_Y;
    int _box_X;
    int _screenW;
    int _screenH;
    SDL_Surface* _p_MenuBox;
    SDL_Surface* _p_SceneBackground;
    SDL_Texture* _p_ScreenTexture;
    SDL_Rect _rctPanel;
    LabelGfx* _p_LabelVersion;
    LabelLinkGfx* _p_homeUrl;
    bool _bMouseInside;
};

#endif
