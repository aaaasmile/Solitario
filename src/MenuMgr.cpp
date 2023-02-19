#include "MenuMgr.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "CompGfx/LabelGfx.h"
#include "CompGfx/LabelLinkGfx.h"
#include "Config.h"
#include "ErrorInfo.h"
#include "GfxUtil.h"
#include "WinTypeGlobal.h"

static const char* g_lpszMsgUrl = "Go to invido.it";
static const char* g_lpszVersion = VERSION "20230131";
static const char* g_lpszIniFontVera = DATA_PREFIX "font/vera.ttf";

static const SDL_Color g_staColor_on = {253, 252, 250};
static const SDL_Color g_color_off = {128, 128, 128};
static const SDL_Color g_color_white = {255, 255, 255};
static const SDL_Color g_color_ombre = {87, 87, 87, 50};
static const SDL_Color g_color_black = {0, 0, 0};
static const SDL_Color g_color_gray = {128, 128, 128};

MenuMgr::MenuMgr() {
    _p_fontAriblk = 0;
    _p_fontVera = 0;
    _p_fontVeraUnderscore = 0;
    _p_ScreenBackbuffer = 0;
    _focusedMenuItem = 0;
    _p_Languages = 0;
    _p_MenuBox = 0;
    _p_SceneBackground = 0;
    _bMouseInside = false;
    _p_homeUrl = NULL;
}

MenuMgr::~MenuMgr() {
    if (_p_MenuBox) {
        SDL_FreeSurface(_p_MenuBox);
        _p_MenuBox = NULL;
    }
    if (_p_ScreenBackbuffer != NULL) {
        SDL_FreeSurface(_p_ScreenBackbuffer);
        _p_ScreenBackbuffer = NULL;
    }
    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
        _p_ScreenTexture = NULL;
    }
    delete _p_homeUrl;
    delete _p_LabelVersion;
}

LPErrInApp MenuMgr::Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer,
                               MenuDelegator& menuDelegator) {
    _menuDlgt = menuDelegator;
    _p_Screen = pScreen;
    _p_sdlRenderer = pRenderer;

    _screenW = _p_Screen->clip_rect.w;
    _box_X = _screenW / 6;
    _screenH = _p_Screen->clip_rect.h;
    _box_Y = _screenH / 5;

    _rctPanel.w = _screenW - _box_X * 2;
    _rctPanel.h = _screenH - _box_Y * 2;
    _rctPanel.x = _box_X;
    _rctPanel.y = _box_Y;

    _p_ScreenBackbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, _p_Screen->w,
                                               _p_Screen->h, 32, 0, 0, 0, 0);
    _p_ScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         _p_Screen->w, _p_Screen->h);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create texture: %s\n",
                                     SDL_GetError());
    }

    _p_fontAriblk = (_menuDlgt.tc)->GetFontAriblk(_menuDlgt.self);
    _p_fontVera = (_menuDlgt.tc)->GetFontVera(_menuDlgt.self);
    _p_Languages = (_menuDlgt.tc)->GetLanguageMan(_menuDlgt.self);

    _p_MenuBox = SDL_CreateRGBSurface(SDL_SWSURFACE, _rctPanel.w, _rctPanel.h,
                                      32, 0, 0, 0, 0);
    SDL_FillRect(_p_MenuBox, NULL,
                 SDL_MapRGBA(_p_Screen->format, 255, 0, 0, 0));
    SDL_SetSurfaceBlendMode(_p_MenuBox, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(_p_MenuBox, 127);  // SDL 2.0

    // link to invido.it
    _p_fontVeraUnderscore = TTF_OpenFont(g_lpszIniFontVera, 11);
    if (_p_fontVeraUnderscore == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     g_lpszIniFontVera, SDL_GetError());
    }
    TTF_SetFontStyle(_p_fontVeraUnderscore, TTF_STYLE_UNDERLINE);
    SDL_Rect rctBt1;
    rctBt1.h = 28;
    rctBt1.w = 150;
    rctBt1.y = _p_Screen->h - rctBt1.h - 20;
    rctBt1.x = _p_Screen->w - rctBt1.w - 20;
    _p_homeUrl = new LabelLinkGfx;
    ClickCb cbNUll = ClickCb{.tc = NULL, .self = NULL};
    _p_homeUrl->Initialize(&rctBt1, _p_ScreenBackbuffer, _p_fontVeraUnderscore,
                           MYIDLABELURL, cbNUll);
    _p_homeUrl->SetState(LabelLinkGfx::INVISIBLE);
    _p_homeUrl->SetUrl(PACKAGE_URL);
    _p_homeUrl->SetWindowText(g_lpszMsgUrl);

    // label version
    _p_LabelVersion = new LabelGfx;
    rctBt1.h = 28;
    rctBt1.w = 150;
    rctBt1.y = _p_homeUrl->PosY() - 20;
    rctBt1.x = _p_homeUrl->PosX();
    _p_LabelVersion->Initialize(&rctBt1, _p_ScreenBackbuffer, _p_fontVera);
    _p_LabelVersion->SetState(LabelGfx::INVISIBLE);
    _p_LabelVersion->SetWindowText(g_lpszVersion);

    SDL_FillRect(_p_ScreenBackbuffer, &_p_ScreenBackbuffer->clip_rect,
                 SDL_MapRGBA(_p_ScreenBackbuffer->format, 0, 0, 0, 0));
    SDL_BlitSurface(_p_Screen, NULL, _p_ScreenBackbuffer, NULL);

    return NULL;
}

void MenuMgr::drawBackground(SDL_Surface* psurf) {
    SDL_Rect rctTarget;
    rctTarget.x = (psurf->w - _p_SceneBackground->w) / 2;
    rctTarget.y = (psurf->h - _p_SceneBackground->h) / 2;
    rctTarget.w = _p_SceneBackground->w;
    rctTarget.h = _p_SceneBackground->h;

    SDL_BlitSurface(_p_SceneBackground, NULL, psurf, &rctTarget);

    _screenW = psurf->clip_rect.w;
    _box_X = _screenW / 6;
    _screenH = psurf->clip_rect.h;
    _box_Y = _screenH / 5;

    Uint32 c_redfg = SDL_MapRGB(psurf->format, 153, 202, 51);

    // content
    GFX_UTIL::DrawStaticSpriteEx(psurf, 0, 0, _rctPanel.w, _rctPanel.h,
                                 _rctPanel.x, _rctPanel.y, _p_MenuBox);

    // header bar
    GFX_UTIL::FillRect(psurf, _box_X, _box_Y - 2, _screenW - _box_X * 2, 38,
                       c_redfg);

    GFX_UTIL::DrawRect(psurf, _box_X - 1, _box_Y - 1, _screenW - _box_X + 1,
                       _screenH - _box_Y + 1, g_color_gray);
    GFX_UTIL::DrawRect(psurf, _box_X - 2, _box_Y - 2, _screenW - _box_X + 2,
                       _screenH - _box_Y + 2, g_color_black);
    GFX_UTIL::DrawRect(psurf, _box_X, _box_Y, _screenW - _box_X,
                       _screenH - _box_Y, g_color_white);
    GFX_UTIL::DrawRect(psurf, _box_X, _box_Y, _screenW - _box_X, _box_Y + 36,
                       g_color_white);
}

LPErrInApp MenuMgr::drawMenuText(SDL_Surface* psurf, const char* text, int x,
                                 int y, SDL_Color& color,
                                 TTF_Font* customfont) {
    GFX_UTIL::DrawString(psurf, text, x + 2, y + 2, g_color_ombre, customfont,
                         true);
    GFX_UTIL::DrawString(psurf, text, x, y, color, customfont, true);
    return NULL;
}

LPErrInApp MenuMgr::HandleRootMenu() {
    LPErrInApp err;
    // show the link url label
    _p_homeUrl->SetState(LabelLinkGfx::VISIBLE);
    _p_LabelVersion->SetState(LabelGfx::VISIBLE);

    SDL_Color c = g_color_white;
    drawBackground(_p_ScreenBackbuffer);
    int iNumItemInMenu = 4;

    // Draw title bar
    err = drawMenuText(
        _p_ScreenBackbuffer,
        _p_Languages->GetStringId(Languages::ID_WELCOMETITLEBAR).c_str(),
        _box_X + 10, _box_Y + 5, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }

    // Play
    if (_focusedMenuItem != 0) {
        c = g_color_off;
    } else {
        c = g_staColor_on;
    }
    err = drawMenuText(_p_ScreenBackbuffer,
                       _p_Languages->GetStringId(Languages::ID_START).c_str(),
                       _box_X + 10, _box_Y + 50, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }
    // Options
    if (_focusedMenuItem != 1) {
        c = g_color_off;
    } else {
        c = g_staColor_on;
    }
    err = drawMenuText(
        _p_ScreenBackbuffer,
        _p_Languages->GetStringId(Languages::ID_MEN_OPTIONS).c_str(),
        _box_X + 10, _box_Y + 90, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }
    // Credits
    if (_focusedMenuItem != 2) {
        c = g_color_off;
    } else {
        c = g_staColor_on;
    }
    err = drawMenuText(_p_ScreenBackbuffer,
                       _p_Languages->GetStringId(Languages::ID_CREDITS).c_str(),
                       _box_X + 10, _box_Y + 130, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }

    // Help
    if (_focusedMenuItem != 3) {
        c = g_color_off;
    } else {
        c = g_staColor_on;
    }
    err = drawMenuText(_p_ScreenBackbuffer,
                       _p_Languages->GetStringId(Languages::ID_MN_HELP).c_str(),
                       _box_X + 10, _box_Y + 170, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }

    // Quit
    if (_focusedMenuItem != 4) {
        c = g_color_off;
    } else {
        c = g_staColor_on;
    }
    err = drawMenuText(_p_ScreenBackbuffer,
                       _p_Languages->GetStringId(Languages::ID_EXIT).c_str(),
                       _box_X + 10, _screenH - _box_Y - 40, c, _p_fontAriblk);
    if (err != NULL) {
        return err;
    }
    SDL_BlitSurface(_p_ScreenBackbuffer, NULL, _p_Screen, NULL);
    updateTextureAsFlipScreen();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            (_menuDlgt.tc)->LeaveMenu(_menuDlgt.self);
            break;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) {
                _focusedMenuItem--;
                if (_focusedMenuItem < 0) {
                    _focusedMenuItem = 0;
                }
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                _focusedMenuItem++;
                if (_focusedMenuItem > iNumItemInMenu) {
                    _focusedMenuItem = iNumItemInMenu;
                }
            }
            if (event.key.keysym.sym == SDLK_RETURN) {
                rootMenuNext();
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                (_menuDlgt.tc)->LeaveMenu(_menuDlgt.self);
            }
        }
        if (event.type == SDL_MOUSEMOTION) {
            if (event.motion.x >= _rctPanel.x &&
                event.motion.x <= _rctPanel.x + _rctPanel.h &&
                event.motion.y >= _rctPanel.y &&
                event.motion.y <= _rctPanel.y + _rctPanel.h) {
                // mouse is inner to the box
                if (event.motion.y >= _box_Y + 90 &&
                    event.motion.y < _box_Y + 130) {
                    _focusedMenuItem = 1;
                } else if (event.motion.y >= _box_Y + 130 &&
                           event.motion.y < _box_Y + 170) {
                    _focusedMenuItem = 2;
                } else if (event.motion.y < _box_Y + 90) {
                    _focusedMenuItem = 0;
                } else if (event.motion.y >= _box_Y + 170 &&
                           event.motion.y < _box_Y + 230) {
                    _focusedMenuItem = 3;
                } else if (event.motion.y >= _screenH - _box_Y - 40) {
                    _focusedMenuItem = 4;
                }
                _bMouseInside = true;
            } else {
                // mouse outside, no focus
                _bMouseInside = false;
            }
            _p_homeUrl->MouseMove(event);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (_bMouseInside) {
                rootMenuNext();
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            _p_homeUrl->MouseUp(event);
        }
    }
    _p_homeUrl->Draw(_p_ScreenBackbuffer);
    _p_LabelVersion->Draw(_p_ScreenBackbuffer);

    SDL_BlitSurface(_p_ScreenBackbuffer, NULL, _p_Screen, NULL);

    updateTextureAsFlipScreen();
    return NULL;
}

void MenuMgr::updateTextureAsFlipScreen() {
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_Screen->pixels,
                      _p_Screen->pitch);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void MenuMgr::rootMenuNext() {
    TRACE_DEBUG("Menu selected %d\n", _focusedMenuItem);
    switch (_focusedMenuItem) {
        case 0:  // Play
            (_menuDlgt.tc)->SetNextMenu(_menuDlgt.self, MENU_GAME);
            break;
        case 1:  // Options
            (_menuDlgt.tc)->SetNextMenu(_menuDlgt.self, MENU_OPTIONS);
            break;
        case 2:  // Credits
            (_menuDlgt.tc)->SetNextMenu(_menuDlgt.self, MENU_CREDITS);
            break;
        case 3:  // Help
            (_menuDlgt.tc)->SetNextMenu(_menuDlgt.self, MENU_HELP);
            break;
        case 4:  // Quit
            (_menuDlgt.tc)->LeaveMenu(_menuDlgt.self);
            break;
    }
}