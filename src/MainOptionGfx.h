#ifndef _MAINOPTION_GFX_H
#define _MAINOPTION_GFX_H

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

#include "ErrorInfo.h"
#include "GameSettings.h"
#include "Traits.h"
#include "WinTypeGlobal.h"

class cButtonGfx;
class Languages;
class cCheckBoxGfx;
class cComboGfx;

using namespace traits;

class MainOptionGfx {
    enum { MYIDOK = 0, MYIDCANCEL = 1, MYIDMUSICCHK = 2, MYIDCOMBOLANG = 3 };

public:
    MainOptionGfx();
    ~MainOptionGfx();

    LPErrInApp Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                          TTF_Font* pFontTxt, TTF_Font* pFontWinCtrl,
                          SDL_Renderer* pRenderer, MenuDelegator& pApp);
    void Show(SDL_Surface* pScene_background);
    void SetCaption(STRING& strCaption) { m_strHeaderText = strCaption; }

private:
    ClickCb prepClickCb();
    ClickCb prepClickComboCb();
    void butCmdClicked(int iButID);
    void comboCmdClicked(int indexSelected);

private:
    SDL_Renderer* m_psdlRenderer;
    SDL_Rect m_rctOptBox;
    SDL_Surface* m_pSurf_Bar;
    SDL_Surface* m_pScreen;
    TTF_Font* m_pFontText;
    TTF_Font* m_pFontCtrl;
    SDL_Color m_colCurrent;
    cButtonGfx* m_pBtOK;
    cButtonGfx* m_pBtCancel;
    bool m_bTerminated;
    int m_iResult;
    STRING m_strHeaderText;
    cCheckBoxGfx* m_pCheckMusic;
    cComboGfx* m_pComboLang;

    Languages* _p_Languages;
    MenuDelegator _menuDlgt;
    GameSettings* _p_GameSettings;
};

#endif
