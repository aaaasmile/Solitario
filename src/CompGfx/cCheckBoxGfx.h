#ifndef _CCHECKBOXGFX__H_
#define _CCHECKBOXGFX__H_

#include <SDL.h>
#include <SDL_ttf.h>

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class cCheckBoxGfx {
private:
    enum { CHECK_W = 16, CHECK_H = 16 };

public:
    cCheckBoxGfx();
    virtual ~cCheckBoxGfx();

public:
    enum eSate {
        VISIBLE,
        INVISIBLE,
    };

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, CheckboxClickCb& fncbClickEvent);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Surface* pScene_background);
    void MouseUp(SDL_Event& event);
    void DrawButton(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { m_bIsEnabled = bVal; }
    void SetState(eSate eVal);
    void RedrawButton(SDL_Surface* pScreen, SDL_Surface* pScene_background);
    bool GetCheckState() { return m_bClickState; }
    void SetCheckState(bool bVal) { m_bClickState = bVal; }

public:
    eSate m_eState;
    SDL_Rect m_rctButt;
    STRING m_strButText;
    bool m_bIsEnabled;
    CheckboxClickCb m_fncbClickEvent;

private:
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    bool m_bClickState;
    int m_iButID;
    SDL_Color m_colBorder;
};

#endif
