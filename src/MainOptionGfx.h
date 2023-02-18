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
                          MenuDelegator& pApp);
    void Show(SDL_Surface* pScene_background, STRING& strCaption);
    void SetCaption(STRING& strCaption) { m_strHeaderText = strCaption; }
    void ButEndOPtClicked(int iButID);
    void CheckboxMusicClicked(bool state);
    void DeckSelectionClicked(int indexSel);

private:
    ClickCb prepClickCb();
    ClickCb prepClickComboCb();
    CheckboxClickCb prepCheckBoxClickMusic();
    ClickCb prepSelectionDeckCb();

private:
    SDL_Renderer* m_psdlRenderer;
    SDL_Rect m_rctOptBox;
    SDL_Surface* m_pSurf_Bar;
    SDL_Surface* m_pScreen;
    TTF_Font* m_pFontText;
    TTF_Font* m_pFontCtrl;
    SDL_Color m_colCurrent;
    ButtonGfx* m_pBtOK;
    bool m_bTerminated;
    int m_iResult;
    STRING m_strHeaderText;
    CheckBoxGfx* m_pCheckMusic;
    ComboGfx* m_pComboLang;
    ComboGfx* m_pComboDeck;
    SDL_Surface* m_pDeckAll[DeckType::NUM_OF_DECK];
    CardGfx m_CardOnEachDeck[3][DeckType::NUM_OF_DECK];

    Languages* _p_Languages;
    MenuDelegator _menuDlgt;
    GameSettings* _p_GameSettings;
    MusicManager* _p_MusicManager;
};

#endif
