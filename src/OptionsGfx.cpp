#include "OptionsGfx.h"

#include <SDL_endian.h>
#include <SDL_image.h>

#include "CompGfx/ButtonGfx.h"
#include "CompGfx/CheckBoxGfx.h"
#include "CompGfx/ComboGfx.h"
#include "GfxUtil.h"
#include "MusicManager.h"

OptionsGfx::OptionsGfx() {
    _p_screen = 0;
    _p_fontText = 0;
    _p_surfBar = 0;
    _p_buttonOK = 0;
    _terminated = false;
    _p_languages = 0;
    _p_MusicManager = 0;
    _p_GameSettings = GAMESET::GetSettings();
}

OptionsGfx::~OptionsGfx() {
    if (_p_surfBar) {
        SDL_FreeSurface(_p_surfBar);
        _p_surfBar = NULL;
    }
    delete _p_buttonOK;
    delete _p_comboLang;
    delete _p_checkMusic;
    delete _p_comboDeck;
    delete _p_comboBackground;
    for (int i = 0; i < eDeckType::NUM_OF_DECK; i++) {
        if (_p_deckAll[i]) {
            SDL_FreeSurface(_p_deckAll[i]);
        }
    }
}

// Prepare the Click() trait
void fncBind_ButtonClicked(void* self, int iVal) {
    OptionsGfx* pOptionsGfx = (OptionsGfx*)self;
    pOptionsGfx->ButEndOPtClicked(iVal);
}

// Buttons, ok and cancel
ClickCb OptionsGfx::prepClickCb() {
#ifndef _MSC_VER
    static VClickCb const tc = {.Click = (&fncBind_ButtonClicked)};

    return (ClickCb){.tc = &tc, .self = this};
#else
    static VClickCb const tc = {(&fncBind_ButtonClicked)};
    ClickCb cb = {&tc, this};
    return cb;
#endif
}

void fncBind_CheckboxMusicClicked(void* self, bool state) {
    OptionsGfx* pOptionsGfx = (OptionsGfx*)self;
    pOptionsGfx->CheckboxMusicClicked(state);
}

// Checkbox music
CheckboxClickCb OptionsGfx::prepCheckBoxClickMusic() {
#ifndef _MSC_VER
    static VCheckboxClickCb const tc = {.Click =
                                            (&fncBind_CheckboxMusicClicked)};

    return (CheckboxClickCb){.tc = &tc, .self = this};
#else
    static VCheckboxClickCb const tc = {(&fncBind_CheckboxMusicClicked)};
    CheckboxClickCb cb = {&tc, this};
    return cb;
#endif
}

LPErrInApp OptionsGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                                  SDL_Renderer* pRenderer,
                                  MusicManager* pMusicMgr,
                                  MenuDelegator& menuDlg) {
    if (pRect == NULL) {
        return ERR_UTIL::ErrorCreate("Rect is null");
    }
    if (pScreen == NULL) {
        return ERR_UTIL::ErrorCreate("pScreen is null");
    }
    _menuDlgt = menuDlg;
    _p_MusicManager = pMusicMgr;
    _rctOptBox = *pRect;
    _p_screen = pScreen;
    _p_fontText = _menuDlgt.tc->GetFontVera(_menuDlgt.self);
    _p_fontCtrl = _menuDlgt.tc->GetFontAriblk(_menuDlgt.self);
    _p_sdlRenderer = pRenderer;
    _p_languages = _menuDlgt.tc->GetLanguageMan(_menuDlgt.self);

    _p_surfBar = SDL_CreateRGBSurface(SDL_SWSURFACE, _rctOptBox.w, _rctOptBox.h,
                                      32, 0, 0, 0, 0);
    SDL_FillRect(_p_surfBar, NULL,
                 SDL_MapRGBA(pScreen->format, 10, 100, 10, 0));
    SDL_SetSurfaceBlendMode(_p_surfBar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(_p_surfBar, 200);  // SDL 2.0

    SDL_Rect rctBt1;
    int iSpace2bt = 20;

    ClickCb cbBtOK = prepClickCb();
    // button OK
    _p_buttonOK = new ButtonGfx;
    rctBt1.w = 120;
    rctBt1.h = 28;
    rctBt1.y = _rctOptBox.y + _rctOptBox.h - 30 - rctBt1.h;
    rctBt1.x =
        (_rctOptBox.w - rctBt1.w) / 2 + _rctOptBox.x + rctBt1.w + iSpace2bt;
    _p_buttonOK->Initialize(&rctBt1, pScreen, _p_fontText, MYIDOK, cbBtOK);
    _p_buttonOK->SetVisibleState(ButtonGfx::INVISIBLE);
    // Combo language
    ClickCb nullCb = {NULL, NULL};
    _p_comboLang = new ComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = _rctOptBox.y + 80;
    rctBt1.x = _rctOptBox.x + 50;
    _p_comboLang->Initialize(&rctBt1, pScreen, _p_fontText, MYIDCOMBOLANG,
                             pRenderer, nullCb);
    _p_comboLang->SetVisibleState(ComboGfx::INVISIBLE);
    // Music
    // check box music
    CheckboxClickCb cbCheckboxMusic = prepCheckBoxClickMusic();
    _p_checkMusic = new CheckBoxGfx;
    rctBt1.w = 180;
    rctBt1.h = 28;
    rctBt1.y = _p_comboLang->PosY() + _p_comboLang->Height() + 20;
    rctBt1.x = _p_comboLang->PosX();
    _p_checkMusic->Initialize(&rctBt1, pScreen, _p_fontText, MYIDMUSICCHK,
                              cbCheckboxMusic);
    _p_checkMusic->SetVisibleState(CheckBoxGfx::INVISIBLE);
    // background
    _p_comboBackground = new ComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = _p_checkMusic->PosY() + _p_checkMusic->Height() + 20;
    rctBt1.x = _p_checkMusic->PosX();
    _p_comboBackground->Initialize(&rctBt1, pScreen, _p_fontText, MYIDCOMBOBACK,
                                   pRenderer, nullCb);
    _p_comboBackground->SetVisibleState(ComboGfx::INVISIBLE);
    // Deck
    // combo deck selection
    _p_comboDeck = new ComboGfx;
    rctBt1.w = 180;
    rctBt1.h = 26;
    rctBt1.y = _p_comboBackground->PosY() + _p_comboBackground->Height() + 30;
    rctBt1.x = _p_comboBackground->PosX();

    _p_comboDeck->Initialize(&rctBt1, pScreen, _p_fontText, MYIDCOMBODECK,
                             pRenderer, nullCb);
    _p_comboDeck->SetVisibleState(ComboGfx::INVISIBLE);

    // init surfaces with all pac decks
    Uint16 pac_w, pac_h;
    DeckType dt;
    LPErrInApp err;
    for (int i = 0; i < eDeckType::NUM_OF_DECK; i++) {
        err = dt.SetTypeIndex(i);
        if (err != NULL) {
            return err;
        }
        err = GFX_UTIL::LoadCardPac(&_p_deckAll[i], dt, &pac_w, &pac_h);
        if (err != NULL) {
            return err;
        }
        int ww = pac_w / 4;
        int hh = pac_h / dt.GetNumCardInSuit();
        int x_pos = rctBt1.x;
        int y_pos = rctBt1.y + rctBt1.h + 20;

        _cardOnEachDeck[0][i].SetIdx(9);
        _cardOnEachDeck[0][i].SetWidth(ww);
        _cardOnEachDeck[0][i].SetHeight(hh);
        _cardOnEachDeck[0][i].SetCardLoc(x_pos, y_pos);
        _cardOnEachDeck[0][i].SetDeckSurface(_p_deckAll[i]);

        _cardOnEachDeck[1][i].SetIdx(7);
        _cardOnEachDeck[1][i].SetWidth(ww);
        _cardOnEachDeck[1][i].SetHeight(hh);
        _cardOnEachDeck[1][i].SetCardLoc(_cardOnEachDeck[0][i].X() + 10 + ww,
                                         y_pos);
        _cardOnEachDeck[1][i].SetDeckSurface(_p_deckAll[i]);

        _cardOnEachDeck[2][i].SetIdx(5);
        _cardOnEachDeck[2][i].SetWidth(ww);
        _cardOnEachDeck[2][i].SetHeight(hh);
        _cardOnEachDeck[2][i].SetCardLoc(_cardOnEachDeck[1][i].X() + 10 + ww,
                                         y_pos);
        _cardOnEachDeck[2][i].SetDeckSurface(_p_deckAll[i]);
    }

    return NULL;
}

LPErrInApp OptionsGfx::Show(SDL_Surface* pScene_background,
                            STRING& strCaption) {
    _headerText = strCaption;
    _terminated = false;
    Uint32 uiInitialTick = SDL_GetTicks();
    Uint32 uiLast_time = uiInitialTick;
    int FPS = 3;

    // button ok
    STRING strTextBt;
    strTextBt = _p_languages->GetStringId(Languages::ID_OK);
    _p_buttonOK->SetButtonText(strTextBt.c_str());
    _p_buttonOK->SetVisibleState(ButtonGfx::VISIBLE);

    // combobox language selection
    STRING strSelectLanguage =
        _p_languages->GetStringId(Languages::ID_CHOOSELANGUA);
    strTextBt = _p_languages->GetStringId(Languages::ID_ITALIANO);
    _p_comboLang->AddLineText(strTextBt.c_str());
    strTextBt = _p_languages->GetStringId(Languages::ID_DIALETMN);
    _p_comboLang->AddLineText(strTextBt.c_str());
    strTextBt = _p_languages->GetStringId(Languages::ID_ENGLISH);
    _p_comboLang->AddLineText(strTextBt.c_str());

    _p_comboLang->SetVisibleState(ComboGfx::VISIBLE);
    _p_comboLang->SelectIndex(_p_GameSettings->CurrentLanguage);

    // checkbox music
    strTextBt = _p_languages->GetStringId(Languages::ID_SOUNDOPT);
    _p_checkMusic->SetWindowText(strTextBt.c_str());
    _p_checkMusic->SetVisibleState(CheckBoxGfx::VISIBLE);
    _p_checkMusic->SetCheckState(_p_GameSettings->MusicEnabled);

    // combobox background selection
    STRING strSelectBackGround =
        _p_languages->GetStringId(Languages::ID_CHOOSEBACKGROUND);
    strTextBt = _p_languages->GetStringId(Languages::ID_COMMESSAGGIO);
    _p_comboBackground->AddLineText(strTextBt.c_str());
    strTextBt = _p_languages->GetStringId(Languages::ID_MANTOVA);
    _p_comboBackground->AddLineText(strTextBt.c_str());
    strTextBt = _p_languages->GetStringId(Languages::ID_BLACK);
    _p_comboBackground->AddLineText(strTextBt.c_str());
    _p_comboBackground->SetVisibleState(ComboGfx::VISIBLE);
    _p_comboBackground->SelectIndex(_p_GameSettings->BackgroundType);

    // combobox deck selection
    STRING strDeckSelectTitle =
        _p_languages->GetStringId(Languages::ID_CHOOSEMAZZO);
    DeckType dt;
    for (int i = 0; i < eDeckType::NUM_OF_DECK; i++) {
        dt.SetTypeIndex(i);
        strTextBt = dt.GetDeckName();
        _p_comboDeck->AddLineText(strTextBt.c_str());
    }
    _p_comboDeck->SetVisibleState(ComboGfx::VISIBLE);
    _p_comboDeck->SelectIndex(_p_GameSettings->DeckTypeVal.GetTypeIndex());

    SDL_Surface* pShadowSrf = SDL_CreateRGBSurface(
        SDL_SWSURFACE, _p_screen->w, _p_screen->h, 32, 0, 0, 0, 0);
    SDL_Texture* pScreenTexture =
        SDL_CreateTextureFromSurface(_p_sdlRenderer, pShadowSrf);
    LPErrInApp err = NULL;
    while (!_terminated) {
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
                _terminated = true;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    err = ButEndOPtClicked(MYIDOK);
                    if (err)
                        return err;
                    break;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    err = ButEndOPtClicked(MYIDCANCEL);
                    if (err)
                        return err;
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                _p_buttonOK->MouseUp(event);
                _p_comboLang->MouseUp(event);
                _p_checkMusic->MouseUp(event);
                _p_comboBackground->MouseUp(event);
                _p_comboDeck->MouseUp(event);
            }
        }

        // the msg box
        GFX_UTIL::DrawStaticSpriteEx(pShadowSrf, 0, 0, _rctOptBox.w,
                                     _rctOptBox.h, _rctOptBox.x, _rctOptBox.y,
                                     _p_surfBar);
        // draw border
        GFX_UTIL::DrawRect(pShadowSrf, _rctOptBox.x - 1, _rctOptBox.y - 1,
                           _rctOptBox.x + _rctOptBox.w + 1,
                           _rctOptBox.y + _rctOptBox.h + 1,
                           GFX_UTIL_COLOR::Gray);
        GFX_UTIL::DrawRect(pShadowSrf, _rctOptBox.x - 2, _rctOptBox.y - 2,
                           _rctOptBox.x + _rctOptBox.w + 2,
                           _rctOptBox.y + _rctOptBox.h + 2,
                           GFX_UTIL_COLOR::Black);
        GFX_UTIL::DrawRect(pShadowSrf, _rctOptBox.x, _rctOptBox.y,
                           _rctOptBox.x + _rctOptBox.w,
                           _rctOptBox.y + _rctOptBox.h, _color);

        // header bar
        SDL_Rect rectHeader;
        Uint32 colorHeader = SDL_MapRGB(_p_screen->format, 153, 202, 51);
        rectHeader.x = _rctOptBox.x + 1;
        rectHeader.y = _rctOptBox.y + 1;
        rectHeader.h = 30;
        rectHeader.w = _rctOptBox.w - 1;
        SDL_FillRect(pShadowSrf, &rectHeader, colorHeader);
        GFX_UTIL::DrawStaticLine(
            pShadowSrf, rectHeader.x, rectHeader.y + rectHeader.h,
            rectHeader.x + rectHeader.w, rectHeader.y + rectHeader.h,
            GFX_UTIL_COLOR::White);
        // text header
        GFX_UTIL::DrawString(pShadowSrf, _headerText.c_str(), rectHeader.x + 10,
                             rectHeader.y, GFX_UTIL_COLOR::White, _p_fontCtrl);

        // Button OK
        _p_buttonOK->DrawButton(pShadowSrf);

        // Combo Language: Label and crontrol
        GFX_UTIL::DrawString(pShadowSrf, strSelectLanguage.c_str(),
                             _p_comboLang->PosX(), _p_comboLang->PosY() - 20,
                             GFX_UTIL_COLOR::Orange, _p_fontText);

        _p_comboLang->DrawButton(pShadowSrf);

        // Checkbox music
        _p_checkMusic->DrawButton(pShadowSrf);

        // Combo Background: Label and control
        GFX_UTIL::DrawString(pShadowSrf, strSelectBackGround.c_str(),
                             _p_comboBackground->PosX(),
                             _p_comboBackground->PosY() - 20,
                             GFX_UTIL_COLOR::Orange, _p_fontText);
        _p_comboBackground->DrawButton(pShadowSrf);

        // Combo Deck: Label and control
        GFX_UTIL::DrawString(pShadowSrf, strDeckSelectTitle.c_str(),
                             _p_comboDeck->PosX(), _p_comboDeck->PosY() - 20,
                             GFX_UTIL_COLOR::Orange, _p_fontText);

        _p_comboDeck->DrawButton(pShadowSrf);

        // Deck example Cards
        int iCurrIndex = _p_comboDeck->GetSlectedIndex();
        _cardOnEachDeck[0][iCurrIndex].DrawCardPac(pShadowSrf);
        _cardOnEachDeck[1][iCurrIndex].DrawCardPac(pShadowSrf);
        _cardOnEachDeck[2][iCurrIndex].DrawCardPac(pShadowSrf);

        // render the dialogbox
        SDL_BlitSurface(pShadowSrf, NULL, _p_screen, NULL);
        SDL_UpdateTexture(pScreenTexture, NULL, _p_screen->pixels,
                          _p_screen->pitch);
        SDL_RenderCopy(_p_sdlRenderer, pScreenTexture, NULL, NULL);
        SDL_RenderPresent(_p_sdlRenderer);

        // synch to frame rate
        Uint32 uiNowTime = SDL_GetTicks();
        if (uiNowTime < uiLast_time + FPS) {
            SDL_Delay(uiLast_time + FPS - uiNowTime);
            uiLast_time = uiNowTime;
        }
    }
    SDL_FreeSurface(pShadowSrf);
    SDL_DestroyTexture(pScreenTexture);
    return NULL;
}

LPErrInApp OptionsGfx::ButEndOPtClicked(int iButID) {
    _terminated = true;
    _result = iButID;
    TRACE("OK options\n");
    Languages::eLangId prevLangId = _p_GameSettings->CurrentLanguage;
    eDeckType prevDeckType = _p_GameSettings->DeckTypeVal.GetType();
    bool prevMusicEnabled = _p_GameSettings->MusicEnabled;
    BackgroundTypeEnum prevBackgroundType = _p_GameSettings->BackgroundType;
    if (_result == MYIDCANCEL) {
        CheckboxMusicClicked(prevMusicEnabled);
        return NULL;
    }

    switch (_p_comboLang->GetSlectedIndex()) {
        case 0:
            _p_GameSettings->CurrentLanguage = Languages::eLangId::LANG_ITA;
            break;
        case 1:
            _p_GameSettings->CurrentLanguage =
                Languages::eLangId::LANG_DIAL_BREDA;
            break;
        case 2:
            _p_GameSettings->CurrentLanguage = Languages::eLangId::LANG_ENG;
            break;
        default:
            break;
    }

    switch (_p_comboBackground->GetSlectedIndex()) {
        case 0:
            _p_GameSettings->BackgroundType = BackgroundTypeEnum::Commessaggio;
            break;
        case 1:
            _p_GameSettings->BackgroundType = BackgroundTypeEnum::Mantova;
            break;
        case 2:
            _p_GameSettings->BackgroundType = BackgroundTypeEnum::Black;
            break;
        default:
            break;
    }

    DeckType dt;
    dt.SetTypeIndex(_p_comboDeck->GetSlectedIndex());
    _p_GameSettings->DeckTypeVal.CopyFrom(dt);
    _p_GameSettings->MusicEnabled = _p_checkMusic->GetCheckState();
    if ((_p_GameSettings->MusicEnabled != prevMusicEnabled) ||
        (_p_GameSettings->DeckTypeVal.GetType() != prevDeckType) ||
        (_p_GameSettings->BackgroundType != prevBackgroundType) ||
        (_p_GameSettings->CurrentLanguage != prevLangId)) {
        TRACE("Settings are changed, save it\n");
        LPErrInApp err = _menuDlgt.tc->SettingsChanged(
            _menuDlgt.self,
            (_p_GameSettings->BackgroundType != prevBackgroundType),
            (_p_GameSettings->CurrentLanguage != prevLangId));
        if (err) {
            return err;
        }
    }
    return NULL;
}

void OptionsGfx::CheckboxMusicClicked(bool state) {
    if (state) {
        _p_MusicManager->PlayCurrentMusic();
    } else {
        _p_MusicManager->StopMusic(500);
    }
}
