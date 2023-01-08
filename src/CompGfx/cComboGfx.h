#ifndef _CCOMBOGFX_H__
#define _CCOMBOGFX_H__

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class cComboGfx {
public:
    enum eSate { VISIBLE, INVISIBLE, PRESSED };

    cComboGfx();
    virtual ~cComboGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, SDL_Renderer* psdlRenderer,
                    ClickCb& fncbClickEvent);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Texture* pScene_background, SDL_Texture* pScreenTexture);
    void MouseUp(SDL_Event& event);
    void DrawButton(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { m_bIsEnabled = bVal; }
    void SetState(eSate eVal);
    void RedrawButton(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                      SDL_Texture* pScreenTexture);
    void AddLineText(LPCSTR strLine);
    void SelectIndex(int iIndex);
    int GetSlectedIndex() { return m_iCurrDataIndex; }
    LPCSTR GetSelectedText() {
        return m_vctDataStrings[m_iCurrDataIndex].c_str();
    }

public:
    eSate m_eState;
    SDL_Rect m_rctButt;
    SDL_Rect m_rctText;
    SDL_Rect m_rctBoxUp;
    SDL_Rect m_rctBoxDown;
    STRING m_strButText;
    bool m_bIsEnabled;
    ClickCb m_fncbClickEvent;

private:
    SDL_Renderer* m_psdlRenderer;
    SDL_Surface* m_pSurf_Bar;
    SDL_Surface* m_pSurf_BoxSel;
    SDL_Surface* m_pSurf_BoxUNSel;
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    int m_iButID;
    VCT_STRING m_vctDataStrings;
    int m_iCurrDataIndex;
};

#endif
