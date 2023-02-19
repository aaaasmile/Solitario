#ifndef _MAINOPTION_GFX_H
#define _MAINOPTION_GFX_H

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

#include "CardGfx.h"
#include "ErrorInfo.h"
#include "GameSettings.h"
#include "Traits.h"
#include "WinTypeGlobal.h"

class ButtonGfx;
class Languages;
class CheckBoxGfx;
class ComboGfx;
class MusicManager;

using namespace traits;

class MainOptionGfx {
    enum {
        MYIDOK = 0,
        MYIDCANCEL = 1,
        MYIDMUSICCHK = 2,
        MYIDCOMBOLANG = 3,
        MYIDCOMBODECK = 4
    };

public:
    MainOptionGfx();
    ~MainOptionGfx();

    LPErrInApp Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                          SDL_Renderer* pRenderer, MusicManager* pMusicMgr,
                          MenuDelegator& menuDlg);
    void Show(SDL_Surface* pScene_background, STRING& strCaption);
    void SetCaption(STRING& strCaption) { _headerText = strCaption; }
    void ButEndOPtClicked(int iButID);
    void CheckboxMusicClicked(bool state);
    void DeckSelectionClicked(int indexSel);

private:
    ClickCb prepClickCb();
    ClickCb prepClickComboCb();
    CheckboxClickCb prepCheckBoxClickMusic();
    ClickCb prepSelectionDeckCb();

private:
    SDL_Renderer* _p_sdlRenderer;
    SDL_Rect _rctOptBox;
    SDL_Surface* _p_surfBar;
    SDL_Surface* _p_screen;
    TTF_Font* _p_fontText;
    TTF_Font* _p_fontCtrl;
    SDL_Color _color;
    ButtonGfx* _p_buttonOK;
    bool _terminated;
    int _result;
    STRING _headerText;
    CheckBoxGfx* _p_checkMusic;
    ComboGfx* _p_comboLang;
    ComboGfx* _p_comboDeck;
    SDL_Surface* _p_deckAll[DeckType::NUM_OF_DECK];
    CardGfx _cardOnEachDeck[3][DeckType::NUM_OF_DECK];

    Languages* _p_languages;
    MenuDelegator _menuDlgt;
    GameSettings* _p_GameSettings;
    MusicManager* _p_MusicManager;
};

#endif
