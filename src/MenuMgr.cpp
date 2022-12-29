#include "MenuMgr.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "CompGfx/cLabelGfx.h"
#include "CompGfx/cLabelLinkGfx.h"
#include "ErrorInfo.h"
#include "GfxUtil.h"
#include "config.h"
#include "WinTypeGlobal.h"

static const char* lpszMsgUrl = "Go to invido.it";
static const char* lpszVersion = VERSION "20221223";
static const char* lpszIniFontVera = "data/font/vera.ttf";

const SDL_Color MenuMgr::staColor_on = {253, 252, 250};
const SDL_Color MenuMgr::staColor_off = {128, 128, 128};
const SDL_Color MenuMgr::staColor_white = {255, 255, 255};
const SDL_Color MenuMgr::staColor_ombre = {87, 87, 87, 50};
const SDL_Color MenuMgr::staColor_black = {0, 0, 0};
const SDL_Color MenuMgr::staColor_gray = {128, 128, 128};

MenuMgr::MenuMgr() {
    _p_font1 = 0;
    _p_font2 = 0;
    _ifocus_valuesM_A = 0;
    _p_Languages = 0;
    _p_MenuBox = 0;
    _p_Scene_background = 0;
    _bMouseInside = FALSE;
}

MenuMgr::~MenuMgr() {
    if (_p_MenuBox) {
        SDL_FreeSurface(_p_MenuBox);
        _p_MenuBox = NULL;
    }
    delete _p_homeUrl;
    delete _p_LabelVersion;
}

// Prepare the Click() trait
void fncBind_LabelClicked(void* self, int iVal) {
    MenuMgr* pApp = (MenuMgr*)self;
    pApp->LabelClicked(iVal);
}

ClickCb MenuMgr::prepClickCb() {
    static VClickCb const tc = {.Click = (&fncBind_LabelClicked)};

    return (ClickCb){.tc = &tc, .self = this};
}

LPErrInApp MenuMgr::Initialize(SDL_Surface* pScreen, SDL_Renderer* pRenderer,
                               MenuDelegator& pApp) {
    _menuDlgt = pApp;
    _p_Screen = pScreen;
    _p_sdlRenderer = pRenderer;

    _iSx = _p_Screen->clip_rect.w;
    _iDebx = _iSx / 6;
    _iSy = _p_Screen->clip_rect.h;
    _iDeby = _iSy / 5;

    _rctPanel.w = _iSx - _iDebx * 2;
    _rctPanel.h = _iSy - _iDeby * 2;
    _rctPanel.x = _iDebx;
    _rctPanel.y = _iDeby;

    _p_font1 = (_menuDlgt.tc)->GetFontAriblk(_menuDlgt.self);
    _p_font2 = (_menuDlgt.tc)->GetFontVera(_menuDlgt.self);
    _p_Languages = (_menuDlgt.tc)->GetLanguageMan(_menuDlgt.self);

    _p_MenuBox = SDL_CreateRGBSurface(SDL_SWSURFACE, _rctPanel.w, _rctPanel.h,
                                      32, 0, 0, 0, 0);
    SDL_FillRect(_p_MenuBox, NULL,
                 SDL_MapRGBA(_p_Screen->format, 255, 0, 0, 0));
    SDL_SetSurfaceBlendMode(_p_MenuBox, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(_p_MenuBox, 127);  // SDL 2.0

    // link to invido.it
    _p_font3 = TTF_OpenFont(lpszIniFontVera, 11);
    if (_p_font3 == 0) {
        return ERR_UTIL::ErrorCreate("Unable to load font %s, error: %s\n",
                                     lpszIniFontVera, SDL_GetError());
    }
    TTF_SetFontStyle(_p_font3, TTF_STYLE_UNDERLINE);
    SDL_Rect rctBt1;
    rctBt1.h = 28;
    rctBt1.w = 150;
    rctBt1.y = _p_Screen->h - rctBt1.h - 20;
    rctBt1.x = _p_Screen->w - rctBt1.w - 20;
    _p_homeUrl = new cLabelLinkGfx;
    ClickCb cb = prepClickCb();
    _p_homeUrl->Initialize(&rctBt1, _p_Screen, _p_font3, MYIDLABELURL,
                           _p_sdlRenderer, cb);
    _p_homeUrl->SetState(cLabelLinkGfx::INVISIBLE);
    _p_homeUrl->SetUrl(PACKAGE_URL);
    _p_homeUrl->SetWindowText(lpszMsgUrl);

    // label version
    _p_LabelVersion = new cLabelGfx;
    rctBt1.h = 28;
    rctBt1.w = 150;
    rctBt1.y = _p_homeUrl->m_rctButt.y - 20;
    rctBt1.x = _p_homeUrl->m_rctButt.x;
    _p_LabelVersion->Initialize(&rctBt1, _p_Screen, _p_font2, MYIDLABELVER,
                                _p_sdlRenderer, cb);
    _p_LabelVersion->SetState(cLabelGfx::INVISIBLE);
    _p_LabelVersion->SetWindowText(lpszVersion);

    return NULL;
}

void MenuMgr::LabelClicked(int iButID) {}

void MenuMgr::drawStaticSpriteEx(int src_x, int src_y, int src_dx, int src_dy,
                                 int dst_x, int dst_y, SDL_Surface* sprite) {
    SDL_Rect src_rec = {src_x, src_y, src_dx, src_dy};
    SDL_Rect dst_rec = {dst_x, dst_y, 0, 0};
    SDL_BlitSurface(sprite, &src_rec, _p_Screen, &dst_rec);
}

void MenuMgr::drawRect(int x, int y, int dx, int dy, SDL_Color c) {
    drawStaticLine(x, y, dx, y, c);
    drawStaticLine(x, y, x, dy, c);
    drawStaticLine(dx, y, dx, dy, c);
    drawStaticLine(x, dy, dx, dy, c);
}

void MenuMgr::drawStaticLine(int x0, int y0, int x1, int y1, SDL_Color color) {
    int d =
        (int)sqrtf(pow((float)(x1 - x0), 2.0f) + pow((float)(y1 - y0), 2.0f));
    static int x = 0, y = 0;
    static int w = _p_Screen->w;
    static int h = _p_Screen->h;
    for (int t = 0; t < d; t++) {
        x = x0 + (x1 - x0) * t / d;
        y = y0 + (y1 - y0) * t / d;
        if ((x >= 0) && (y >= 0) && (x < w) && (y < h)) {
            setPixel(_p_Screen, x, y, color);
        }
    }
}

void MenuMgr::setPixel(SDL_Surface* pSurface, int x, int y, SDL_Color color) {
    Uint32 col = SDL_MapRGB(pSurface->format, color.r, color.g, color.b);

    char* pPosition = (char*)pSurface->pixels;

    pPosition += (pSurface->pitch * y);

    pPosition += (pSurface->format->BytesPerPixel * x);

    memcpy(pPosition, &col, pSurface->format->BytesPerPixel);
}

void MenuMgr::fillRect(int x0, int y0, int width, int height, Uint32 color) {
    SDL_Rect rect = {x0, y0, width, height};
    SDL_FillRect(_p_Screen, &rect, color);
}

void MenuMgr::drawBackground() {
    SDL_Rect rctTarget;
    rctTarget.x = (_p_Screen->w - _p_Scene_background->w) / 2;
    rctTarget.y = (_p_Screen->h - _p_Scene_background->h) / 2;
    rctTarget.w = _p_Scene_background->w;
    rctTarget.h = _p_Scene_background->h;

    SDL_BlitSurface(_p_Scene_background, NULL, _p_Screen, &rctTarget);

    _iSx = _p_Screen->clip_rect.w;
    _iDebx = _iSx / 6;
    _iSy = _p_Screen->clip_rect.h;
    _iDeby = _iSy / 5;

    Uint32 c_redfg = SDL_MapRGB(_p_Screen->format, 153, 202, 51);

    // don't invert, because content overwrite header
    // content
    GFX_UTIL::DrawStaticSpriteEx(_p_Screen, 0, 0, _rctPanel.w, _rctPanel.h,
                                 _rctPanel.x, _rctPanel.y, _p_MenuBox);

    // header bar
    fillRect(_iDebx, _iDeby - 2, _iSx - _iDebx * 2, 38, c_redfg);

    drawRect(_iDebx - 1, _iDeby - 1, _iSx - _iDebx + 1, _iSy - _iDeby + 1,
             staColor_gray);
    drawRect(_iDebx - 2, _iDeby - 2, _iSx - _iDebx + 2, _iSy - _iDeby + 2,
             staColor_black);
    drawRect(_iDebx, _iDeby, _iSx - _iDebx, _iSy - _iDeby, staColor_white);
    drawRect(_iDebx, _iDeby, _iSx - _iDebx, _iDeby + 36, staColor_white);
}

LPErrInApp MenuMgr::drawStringSH(const char* tmp, int x, int y,
                                 SDL_Color& color, TTF_Font* customfont) {
    int tx, ty;
    TTF_SizeText(customfont, tmp, &tx, &ty);
    SDL_Surface* s = TTF_RenderText_Blended(customfont, tmp, staColor_ombre);
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate("Error TTF_RenderText_Blended: %s\n",
                                     SDL_GetError());
    }
    drawStaticSpriteEx(0, 0, tx, ty, x + 2, y + 2, s);
    SDL_FreeSurface(s);
    s = TTF_RenderText_Blended(customfont, tmp, color);
    if (s == NULL) {
        return ERR_UTIL::ErrorCreate("Error TTF_RenderText_Blended: %s\n",
                                     SDL_GetError());
    }
    drawStaticSpriteEx(0, 0, tx, ty, x, y, s);
    SDL_FreeSurface(s);
    return NULL;
}

LPErrInApp MenuMgr::HandleRootMenu() {
    LPErrInApp err;
    // show the link url label
    _p_homeUrl->SetState(cLabelLinkGfx::VISIBLE);
    _p_LabelVersion->SetState(cLabelGfx::VISIBLE);

    SDL_Color c = staColor_white;
    drawBackground();
    int iNumItemInMenu = 4;

    // Draw title bar
    err = drawStringSH(
        _p_Languages->GetStringId(Languages::ID_WELCOMETITLEBAR).c_str(),
        _iDebx + 10, _iDeby + 5, c, _p_font1);
    if (err != NULL) {
        return err;
    }

    // Play
    if (_ifocus_valuesM_A != 0) {
        c = staColor_off;
    } else {
        c = staColor_on;
    }
    err = drawStringSH(_p_Languages->GetStringId(Languages::ID_START).c_str(),
                       _iDebx + 10, _iDeby + 50, c, _p_font1);
    if (err != NULL) {
        return err;
    }
    // Options
    if (_ifocus_valuesM_A != 1) {
        c = staColor_off;
    } else {
        c = staColor_on;
    }
    err = drawStringSH(
        _p_Languages->GetStringId(Languages::ID_MEN_OPTIONS).c_str(),
        _iDebx + 10, _iDeby + 90, c, _p_font1);
    if (err != NULL) {
        return err;
    }
    // Credits
    if (_ifocus_valuesM_A != 2) {
        c = staColor_off;
    } else {
        c = staColor_on;
    }
    err = drawStringSH(_p_Languages->GetStringId(Languages::ID_CREDITS).c_str(),
                       _iDebx + 10, _iDeby + 130, c, _p_font1);
    if (err != NULL) {
        return err;
    }

    // Help
    if (_ifocus_valuesM_A != 3) {
        c = staColor_off;
    } else {
        c = staColor_on;
    }
    err = drawStringSH(_p_Languages->GetStringId(Languages::ID_MN_HELP).c_str(),
                       _iDebx + 10, _iDeby + 170, c, _p_font1);
    if (err != NULL) {
        return err;
    }

    // Quit
    if (_ifocus_valuesM_A != 4) {
        c = staColor_off;
    } else {
        c = staColor_on;
    }
    err = drawStringSH(_p_Languages->GetStringId(Languages::ID_EXIT).c_str(),
                       _iDebx + 10, _iSy - _iDeby - 40, c, _p_font1);
    if (err != NULL) {
        return err;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            (_menuDlgt.tc)->LeaveMenu(_menuDlgt.self);
            break;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) {
                _ifocus_valuesM_A--;
                if (_ifocus_valuesM_A < 0) {
                    _ifocus_valuesM_A = 0;
                }
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                _ifocus_valuesM_A++;
                if (_ifocus_valuesM_A > iNumItemInMenu) {
                    _ifocus_valuesM_A = iNumItemInMenu;
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
                if (event.motion.y >= _iDeby + 90 &&
                    event.motion.y < _iDeby + 130) {
                    _ifocus_valuesM_A = 1;
                } else if (event.motion.y >= _iDeby + 130 &&
                           event.motion.y < _iDeby + 170) {
                    _ifocus_valuesM_A = 2;
                } else if (event.motion.y < _iDeby + 90) {
                    _ifocus_valuesM_A = 0;
                } else if (event.motion.y >= _iDeby + 170 &&
                           event.motion.y < _iDeby + 230) {
                    _ifocus_valuesM_A = 3;
                } else if (event.motion.y >= _iSy - _iDeby - 40) {
                    _ifocus_valuesM_A = 4;
                }
                _bMouseInside = TRUE;
            } else {
                // mouse outside, no focus
                _bMouseInside = FALSE;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (_bMouseInside) {
                rootMenuNext();
            }
        } else if (event.type == SDL_MOUSEBUTTONUP) {
            _p_homeUrl->MouseUp(event);
        }
    }
    _p_homeUrl->Draw(_p_Screen);
    _p_LabelVersion->Draw(_p_Screen);
    return NULL;
}

void MenuMgr::rootMenuNext() {
    TRACE("Menu selected %d", _ifocus_valuesM_A);
    switch (_ifocus_valuesM_A) {
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