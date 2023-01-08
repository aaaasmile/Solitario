
#include "MainOptionGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>

#include "CompGfx/cButtonGfx.h"
#include "CompGfx/cCheckBoxGfx.h"
#include "CompGfx/cComboGfx.h"
#include "GfxUtil.h"

MainOptionGfx::MainOptionGfx() {
    m_pScreen = 0;
    m_pFontText = 0;
    m_pSurf_Bar = 0;
    m_pBtOK = 0;
    m_pBtCancel = 0;
    m_bTerminated = false;
    _p_Languages = 0;
    _p_GameSettings = GAMESET::GetSettings();
}

MainOptionGfx::~MainOptionGfx() {
    if (m_pSurf_Bar) {
        SDL_FreeSurface(m_pSurf_Bar);
        m_pSurf_Bar = NULL;
    }
    delete m_pBtOK;
    delete m_pBtCancel;
}

// Prepare the Click() trait
void fncBind_ButtonClicked(void* self, int iVal) {
    MainOptionGfx* pApp = (MainOptionGfx*)self;
    pApp->butCmdClicked(iVal);
}

// Buttons, ok and cancel
ClickCb MainOptionGfx::prepClickCb() {
    static VClickCb const tc = {.Click = (&fncBind_ButtonClicked)};

    return (ClickCb){.tc = &tc, .self = this};
}

void fncBind_ComboboxClicked(void* self, int iVal) {
    MainOptionGfx* pApp = (MainOptionGfx*)self;
    pApp->comboCmdClicked(iVal);
}
// combobox index selected
ClickCb MainOptionGfx::prepClickComboCb() {
    static VClickCb const tc = {.Click = (&fncBind_ComboboxClicked)};

    return (ClickCb){.tc = &tc, .self = this};
}

LPErrInApp MainOptionGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                                     TTF_Font* pFontTxt, TTF_Font* pFontWinCtrl,
                                     SDL_Renderer* pRenderer,
                                     MenuDelegator& pApp) {
    if (pRect == NULL) {
        return ERR_UTIL::ErrorCreate("Rect is null");
    }
    if (pScreen == NULL) {
        return ERR_UTIL::ErrorCreate("pScreen is null");
    }
    _menuDlgt = pApp;
    m_rctOptBox = *pRect;
    m_pScreen = pScreen;
    m_pFontText = pFontTxt;
    m_pFontCtrl = pFontWinCtrl;
    m_psdlRenderer = pRenderer;
    _p_Languages = (_menuDlgt.tc)->GetLanguageMan(_menuDlgt.self);

    m_pSurf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctOptBox.w,
                                       m_rctOptBox.h, 32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_Bar, NULL,
                 SDL_MapRGBA(pScreen->format, 10, 100, 10, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_Bar, 200);  // SDL 2.0

    SDL_Rect rctBt1;
    int iSpace2bt = 20;

    ClickCb cbBtOK_Cancel = prepClickCb();
    m_pBtOK = new cButtonGfx;
    rctBt1.w = 120;
    rctBt1.h = 28;
    rctBt1.y = m_rctOptBox.y + m_rctOptBox.h - 10 - rctBt1.h;
    rctBt1.x =
        (m_rctOptBox.w - rctBt1.w) / 2 + m_rctOptBox.x - rctBt1.w - iSpace2bt;
    m_pBtOK->Initialize(&rctBt1, pScreen, m_pFontText, MYIDOK, pRenderer,
                        cbBtOK_Cancel);
    m_pBtOK->SetState(cButtonGfx::INVISIBLE);

    m_pBtCancel = new cButtonGfx;
    rctBt1.w = 120;
    rctBt1.h = 28;
    rctBt1.y = m_rctOptBox.y + m_rctOptBox.h - 10 - rctBt1.h;
    rctBt1.x =
        (m_rctOptBox.w - rctBt1.w) / 2 + m_rctOptBox.x + rctBt1.w + iSpace2bt;
    m_pBtCancel->Initialize(&rctBt1, pScreen, m_pFontText, MYIDCANCEL,
                            pRenderer, cbBtOK_Cancel);
    m_pBtCancel->SetState(cButtonGfx::INVISIBLE);

    ClickCb cbCombo = prepClickComboCb();
    m_pComboLang = new cComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = m_rctOptBox.y + 80;
    rctBt1.x = m_rctOptBox.x + 50;
    m_pComboLang->Initialize(&rctBt1, pScreen, m_pFontText, MYIDCOMBOLANG,
                             pRenderer, cbCombo);
    m_pComboLang->SetState(cComboGfx::INVISIBLE);

    return NULL;
}

void MainOptionGfx::Show(SDL_Surface* pScene_background) {
    int iRes = 0;
    m_bTerminated = false;
    Uint32 uiInitialTick = SDL_GetTicks();
    Uint32 uiLast_time = uiInitialTick;
    int FPS = 3;

    // button ok
    STRING strTextBt;
    strTextBt = _p_Languages->GetStringId(Languages::ID_OK);
    m_pBtOK->SetWindowText(strTextBt.c_str());
    m_pBtOK->SetState(cButtonGfx::VISIBLE);

    // button cancel
    strTextBt = _p_Languages->GetStringId(Languages::ID_CANCEL);
    m_pBtCancel->SetWindowText(strTextBt.c_str());
    m_pBtCancel->SetState(cButtonGfx::VISIBLE);

    // combobox language selection
    STRING strDeckSelectTitle =
        _p_Languages->GetStringId(Languages::ID_CHOOSELANGUA);
    strTextBt = _p_Languages->GetStringId(Languages::ID_ITALIANO);
    m_pComboLang->AddLineText(strTextBt.c_str());
    strTextBt = _p_Languages->GetStringId(Languages::ID_DIALETMN);
    m_pComboLang->AddLineText(strTextBt.c_str());
    strTextBt = _p_Languages->GetStringId(Languages::ID_ENGLISH);
    m_pComboLang->AddLineText(strTextBt.c_str());

    m_pComboLang->SetState(cComboGfx::VISIBLE);
    m_pComboLang->SelectIndex(_p_GameSettings->eLanguageCurrent);

    // checkbox music
    strTextBt = _p_Languages->GetStringId(Languages::ID_SOUNDOPT);
    m_pCheckMusic->SetWindowText(strTextBt.c_str());
    m_pCheckMusic->SetState(cCheckBoxGfx::VISIBLE);
    m_pCheckMusic->SetCheckState(_p_GameSettings->bMusicEnabled);

    SDL_Surface* pShadowSrf = SDL_CreateRGBSurface(
        SDL_SWSURFACE, m_pScreen->w, m_pScreen->h, 32, 0, 0, 0, 0);
    SDL_Texture* pScreenTexture =
        SDL_CreateTextureFromSurface(m_psdlRenderer, pShadowSrf);
    while (!m_bTerminated) {
        SDL_BlitSurface(pScene_background, NULL, pShadowSrf, NULL);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_bTerminated = true;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    butCmdClicked(MYIDOK);
                    break;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    butCmdClicked(MYIDCANCEL);
                    break;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                m_pBtCancel->MouseUp(event);
                m_pBtOK->MouseUp(event);
                m_pComboLang->MouseUp(event);
                m_pCheckMusic->MouseUp(event);
            }
        }

        // the msg box
        GFX_UTIL::DrawStaticSpriteEx(pShadowSrf, 0, 0, m_rctOptBox.w,
                                     m_rctOptBox.h, m_rctOptBox.x,
                                     m_rctOptBox.y, m_pSurf_Bar);
        // draw border
        GFX_UTIL::DrawRect(pShadowSrf, m_rctOptBox.x - 1, m_rctOptBox.y - 1,
                           m_rctOptBox.x + m_rctOptBox.w + 1,
                           m_rctOptBox.y + m_rctOptBox.h + 1,
                           GFX_UTIL_COLOR::Gray);
        GFX_UTIL::DrawRect(pShadowSrf, m_rctOptBox.x - 2, m_rctOptBox.y - 2,
                           m_rctOptBox.x + m_rctOptBox.w + 2,
                           m_rctOptBox.y + m_rctOptBox.h + 2,
                           GFX_UTIL_COLOR::Black);
        GFX_UTIL::DrawRect(pShadowSrf, m_rctOptBox.x, m_rctOptBox.y,
                           m_rctOptBox.x + m_rctOptBox.w,
                           m_rctOptBox.y + m_rctOptBox.h, m_colCurrent);

        // header bar
        SDL_Rect rectHeader;
        Uint32 colorHeader = SDL_MapRGB(m_pScreen->format, 153, 202, 51);
        rectHeader.x = m_rctOptBox.x + 1;
        rectHeader.y = m_rctOptBox.y + 1;
        rectHeader.h = 30;
        rectHeader.w = m_rctOptBox.w - 1;
        SDL_FillRect(pShadowSrf, &rectHeader, colorHeader);
        GFX_UTIL::DrawStaticLine(
            pShadowSrf, rectHeader.x, rectHeader.y + rectHeader.h,
            rectHeader.x + rectHeader.w, rectHeader.y + rectHeader.h,
            GFX_UTIL_COLOR::White);
        // text header
        GFX_UTIL::DrawString(pShadowSrf, m_strHeaderText.c_str(),
                             rectHeader.x + 10, rectHeader.y,
                             GFX_UTIL_COLOR::White, m_pFontCtrl, false);

        // draw buttons
        m_pBtOK->DrawButton(pShadowSrf);
        m_pBtCancel->DrawButton(pShadowSrf);

        // deck selection label
        GFX_UTIL::DrawString(pShadowSrf, strDeckSelectTitle.c_str(),
                             m_pComboLang->m_rctButt.x,
                             m_pComboLang->m_rctButt.y - 20,
                             GFX_UTIL_COLOR::Orange, m_pFontText, false);

        // draw the deck combobox selection
        m_pComboLang->DrawButton(pShadowSrf);

        // render the dialogbox
        SDL_BlitSurface(pShadowSrf, NULL, m_pScreen, NULL);
        SDL_UpdateTexture(pScreenTexture, NULL, m_pScreen->pixels,
                          m_pScreen->pitch);
        SDL_RenderCopy(m_psdlRenderer, pScreenTexture, NULL, NULL);
        SDL_RenderPresent(m_psdlRenderer);

        // synch to frame rate
        Uint32 uiNowTime = SDL_GetTicks();
        if (uiNowTime < uiLast_time + FPS) {
            SDL_Delay(uiLast_time + FPS - uiNowTime);
            uiLast_time = uiNowTime;
        }
    }
    SDL_FreeSurface(pShadowSrf);
    SDL_DestroyTexture(pScreenTexture);
}

void MainOptionGfx::butCmdClicked(int iButID) {
    if (!m_bTerminated) {
        m_bTerminated = true;
        m_iResult = iButID;
        if (iButID == MYIDOK) {
            switch (m_pComboLang->GetSlectedIndex()) {
                case 0:
                    _p_GameSettings->eLanguageCurrent =
                        Languages::eLangId::LANG_ITA;
                    break;
                case 1:
                    _p_GameSettings->eLanguageCurrent =
                        Languages::eLangId::LANG_DIAL_BREDA;
                    break;
                case 2:
                    _p_GameSettings->eLanguageCurrent =
                        Languages::eLangId::LANG_ENG;
                    break;
                default:
                    break;
            }
            _p_GameSettings->bMusicEnabled = m_pCheckMusic->GetCheckState();
        }
    }
}

void MainOptionGfx::comboCmdClicked(int indexSelected) {}
