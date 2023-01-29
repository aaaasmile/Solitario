
#include "MainOptionGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>

#include "CompGfx/cButtonGfx.h"
#include "CompGfx/cCheckBoxGfx.h"
#include "CompGfx/cComboGfx.h"
#include "GfxUtil.h"
#include "MusicManager.h"

MainOptionGfx::MainOptionGfx() {
    m_pScreen = 0;
    m_pFontText = 0;
    m_pSurf_Bar = 0;
    m_pBtOK = 0;
    m_bTerminated = false;
    _p_Languages = 0;
    _p_MusicManager = 0;
    _p_GameSettings = GAMESET::GetSettings();
}

MainOptionGfx::~MainOptionGfx() {
    if (m_pSurf_Bar) {
        SDL_FreeSurface(m_pSurf_Bar);
        m_pSurf_Bar = NULL;
    }
    delete m_pBtOK;
    delete m_pComboLang;
    delete m_pCheckMusic;
}

// Prepare the Click() trait
void fncBind_ButtonClicked(void* self, int iVal) {
    MainOptionGfx* pApp = (MainOptionGfx*)self;
    pApp->ButEndOPtClicked(iVal);
}

// Buttons, ok and cancel
ClickCb MainOptionGfx::prepClickCb() {
    static VClickCb const tc = {.Click = (&fncBind_ButtonClicked)};

    return (ClickCb){.tc = &tc, .self = this};
}

void fncBind_CheckboxMusicClicked(void* self, bool state) {
    MainOptionGfx* pApp = (MainOptionGfx*)self;
    pApp->CheckboxMusicClicked(state);
}

// Checkbox music
CheckboxClickCb MainOptionGfx::prepCheckBoxClickMusic() {
    static VCheckboxClickCb const tc = {.Click =
                                            (&fncBind_CheckboxMusicClicked)};

    return (CheckboxClickCb){.tc = &tc, .self = this};
}

// deck selection
void fncBind_ComboDeckClicked(void* self, int index) {
    MainOptionGfx* pApp = (MainOptionGfx*)self;
    pApp->DeckSelectionClicked(index);
}

ClickCb MainOptionGfx::prepSelectionDeckCb() {
    static VClickCb const tc = {.Click = (&fncBind_ComboDeckClicked)};
    return (ClickCb){.tc = &tc, .self = this};
}

LPErrInApp MainOptionGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                                     SDL_Renderer* pRenderer,
                                     MusicManager* pMusicMgr,
                                     MenuDelegator& pApp) {
    if (pRect == NULL) {
        return ERR_UTIL::ErrorCreate("Rect is null");
    }
    if (pScreen == NULL) {
        return ERR_UTIL::ErrorCreate("pScreen is null");
    }
    _menuDlgt = pApp;
    _p_MusicManager = pMusicMgr;
    m_rctOptBox = *pRect;
    m_pScreen = pScreen;
    m_pFontText = _menuDlgt.tc->GetFontVera(_menuDlgt.self);
    m_pFontCtrl = _menuDlgt.tc->GetFontAriblk(_menuDlgt.self);
    m_psdlRenderer = pRenderer;
    _p_Languages = _menuDlgt.tc->GetLanguageMan(_menuDlgt.self);

    m_pSurf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctOptBox.w,
                                       m_rctOptBox.h, 32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_Bar, NULL,
                 SDL_MapRGBA(pScreen->format, 10, 100, 10, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_Bar, 200);  // SDL 2.0

    SDL_Rect rctBt1;
    int iSpace2bt = 20;

    ClickCb cbBtOK_Cancel = prepClickCb();
    // button OK
    m_pBtOK = new cButtonGfx;
    rctBt1.w = 120;
    rctBt1.h = 28;
    rctBt1.y = m_rctOptBox.y + m_rctOptBox.h - 10 - rctBt1.h;
    rctBt1.x =
        (m_rctOptBox.w - rctBt1.w) / 2 + m_rctOptBox.x + rctBt1.w + iSpace2bt;
    m_pBtOK->Initialize(&rctBt1, pScreen, m_pFontText, MYIDOK, pRenderer,
                        cbBtOK_Cancel);
    m_pBtOK->SetState(cButtonGfx::INVISIBLE);
    // Combo language
    ClickCb nullCb = {.tc = NULL, .self = NULL};
    m_pComboLang = new cComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = m_rctOptBox.y + 80;
    rctBt1.x = m_rctOptBox.x + 50;
    m_pComboLang->Initialize(&rctBt1, pScreen, m_pFontText, MYIDCOMBOLANG,
                             pRenderer, nullCb);
    m_pComboLang->SetState(cComboGfx::INVISIBLE);
    // Music
    // check box music
    CheckboxClickCb cbCheckboxMusic = prepCheckBoxClickMusic();
    m_pCheckMusic = new cCheckBoxGfx;
    rctBt1.w = 180;
    rctBt1.h = 28;
    rctBt1.y = m_pComboLang->m_rctButt.y + m_pComboLang->m_rctButt.h + 20;
    rctBt1.x = m_pComboLang->m_rctButt.x;
    m_pCheckMusic->Initialize(&rctBt1, pScreen, m_pFontText, MYIDMUSICCHK,
                              cbCheckboxMusic);
    m_pCheckMusic->SetState(cCheckBoxGfx::INVISIBLE);
    // Deck
    // combo deck selection
    ClickCb deckSelCb = prepSelectionDeckCb();
    m_pComboDeck = new cComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = m_pCheckMusic->m_rctButt.y + m_pCheckMusic->m_rctButt.h + 20;
    rctBt1.x = m_pCheckMusic->m_rctButt.x;

    m_pComboDeck->Initialize(&rctBt1, pScreen, m_pFontText, MYIDCOMBODECK,
                             pRenderer, deckSelCb);
    m_pComboDeck->SetState(cComboGfx::INVISIBLE);

    // init deck surface
    Uint16 pac_w, pac_h;
    DeckType* pDt = new DeckType();
    for (int i = 0; i < DeckType::NUM_OF_DECK; i++) {
        pDt->SetType((DeckType::eDeckType)i);
        GFX_UTIL::LoadCardPac(&m_pDeckAll[i], *pDt, &pac_w, &pac_h);
        m_CardOnEachDeck[0][i].SetWidth(pac_w / 4);
        m_CardOnEachDeck[0][i].SetHeight(pac_h / 10);
        m_CardOnEachDeck[0][i].SetCardLoc(rctBt1.x, rctBt1.y + rctBt1.h + 50);
        m_CardOnEachDeck[0][i].SetIdx(9);
        m_CardOnEachDeck[0][i].SetDeckSurface(m_pDeckAll[i],
                                              m_CardOnEachDeck[0][i].Width(),
                                              m_CardOnEachDeck[0][i].Height());

        m_CardOnEachDeck[1][i].SetDeckSurface(m_pDeckAll[i],
                                              m_CardOnEachDeck[0][i].Width(),
                                              m_CardOnEachDeck[0][i].Height());
        m_CardOnEachDeck[1][i].SetCardLoc(
            m_CardOnEachDeck[0][i].X() + 10 + m_CardOnEachDeck[0][i].Width(),
            rctBt1.y + rctBt1.h + 50);
        m_CardOnEachDeck[1][i].SetIdx(7);

        m_CardOnEachDeck[2][i].SetDeckSurface(m_pDeckAll[i],
                                              m_CardOnEachDeck[0][i].Width(),
                                              m_CardOnEachDeck[0][i].Height());
        m_CardOnEachDeck[2][i].SetCardLoc(
            m_CardOnEachDeck[1][i].X() + 10 + m_CardOnEachDeck[1][i].Width(),
            rctBt1.y + rctBt1.h + 50);
        m_CardOnEachDeck[2][i].SetIdx(5);
    }
    delete pDt;

    return NULL;
}

void MainOptionGfx::Show(SDL_Surface* pScene_background, STRING& strCaption) {
    m_strHeaderText = strCaption;
    m_bTerminated = false;
    Uint32 uiInitialTick = SDL_GetTicks();
    Uint32 uiLast_time = uiInitialTick;
    int FPS = 3;

    // button ok
    STRING strTextBt;
    strTextBt = _p_Languages->GetStringId(Languages::ID_OK);
    m_pBtOK->SetWindowText(strTextBt.c_str());
    m_pBtOK->SetState(cButtonGfx::VISIBLE);

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
        // center the background
        SDL_FillRect(pShadowSrf, &pShadowSrf->clip_rect,
                     SDL_MapRGBA(pShadowSrf->format, 0, 0, 0, 0));
        SDL_Rect rctTarget;
        rctTarget.x = (pShadowSrf->w - pScene_background->w) / 2;
        rctTarget.y = (pShadowSrf->h - pScene_background->h) / 2;
        rctTarget.w = pScene_background->w;
        rctTarget.h = pScene_background->h;
        SDL_BlitSurface(pScene_background, NULL, pShadowSrf, &rctTarget);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_bTerminated = true;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    ButEndOPtClicked(MYIDOK);
                    break;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    ButEndOPtClicked(MYIDCANCEL);
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
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

        // deck selection label
        GFX_UTIL::DrawString(pShadowSrf, strDeckSelectTitle.c_str(),
                             m_pComboLang->m_rctButt.x,
                             m_pComboLang->m_rctButt.y - 20,
                             GFX_UTIL_COLOR::Orange, m_pFontText, false);

        // draw the deck combobox selection
        m_pComboLang->DrawButton(pShadowSrf);
        // draw checkbox music
        m_pCheckMusic->DrawButton(pShadowSrf);

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

void MainOptionGfx::ButEndOPtClicked(int iButID) {
    m_bTerminated = true;
    m_iResult = iButID;
    TRACE("OK options");
    Languages::eLangId prevLangId = _p_GameSettings->eLanguageCurrent;
    bool prevMusicEnabled = _p_GameSettings->bMusicEnabled;
    switch (m_pComboLang->GetSlectedIndex()) {
        case 0:
            _p_GameSettings->eLanguageCurrent = Languages::eLangId::LANG_ITA;
            break;
        case 1:
            _p_GameSettings->eLanguageCurrent =
                Languages::eLangId::LANG_DIAL_BREDA;
            break;
        case 2:
            _p_GameSettings->eLanguageCurrent = Languages::eLangId::LANG_ENG;
            break;
        default:
            break;
    }
    _p_GameSettings->bMusicEnabled = m_pCheckMusic->GetCheckState();
    if ((_p_GameSettings->bMusicEnabled != prevMusicEnabled) ||
        (_p_GameSettings->eLanguageCurrent != prevLangId)) {
        TRACE("Settings are changed, save it");
        _menuDlgt.tc->PersistSettings(_menuDlgt.self);
    }
}

void MainOptionGfx::CheckboxMusicClicked(bool state) {
    if (state) {
        _p_MusicManager->PlayCurrentMusic();
    } else {
        _p_MusicManager->StopMusic();
    }
}

void MainOptionGfx::DeckSelectionClicked(int indexSel) {}
