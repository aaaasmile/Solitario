#ifndef _CBUTTON_GFX_H
#define _CBUTTON_GFX_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class cButtonGfx {
public:
    enum VisbleState { VISIBLE, INVISIBLE, PRESSED };
    enum BitmapType { TEXT_BUTTON, BITMAP_BUTTON };
    enum MouseState { OUTSIDE, INSIDE };

    cButtonGfx();
    ~cButtonGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, ClickCb& fncbClickEvent);
    void SetWindowText(LPCSTR strCaption) { m_strButText = strCaption; }
    bool MouseMove(SDL_Event& event);
    bool MouseUp(SDL_Event& event);
    bool MouseDown(SDL_Event& event);
    void DrawButton(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { m_bIsEnabled = bVal; }
    void SetState(VisbleState eVal);
    void SetButtonType(BitmapType eTypevalue) { m_eButType = eTypevalue; }
    void SetButBitmapSurfaces(SDL_Surface* pS1, SDL_Surface* pS2,
                              SDL_Surface* pS3) {
        m_pSurf_Bitmap_Act1 = pS1;
        m_pSurf_Bitmap_MOv2 = pS2;
        m_pSurf_Bitmap_Pus3 = pS3;
    }

private:
    void drawBtAsBitmap(SDL_Surface* pScreen);

public:
    VisbleState m_eState;
    SDL_Rect m_rctButt;
    STRING m_strButText;
    bool m_bIsEnabled;
    ClickCb m_fncbClickEvent;

private:
    SDL_Surface* m_pSurf_Bar;
    TTF_Font* m_pFontText;
    SDL_Color m_colCurrent;
    int m_iButID;
    bool m_bMouseIsDown;
    BitmapType m_eButType;
    SDL_Surface* m_pSurf_Bitmap_Act1;
    SDL_Surface* m_pSurf_Bitmap_MOv2;
    SDL_Surface* m_pSurf_Bitmap_Pus3;
    MouseState _mouseState;
};

#endif
