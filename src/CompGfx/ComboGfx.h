#ifndef _CCOMBOGFX_H__
#define _CCOMBOGFX_H__

#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class ComboGfx {
public:
    enum VisbleState { VISIBLE, INVISIBLE, PRESSED };

    ComboGfx();
    virtual ~ComboGfx();

    void Initialize(SDL_Rect* pRect, SDL_Surface* pScreen, TTF_Font* pFont,
                    int iButID, SDL_Renderer* psdlRenderer,
                    ClickCb& fncbClickEvent);
    void SetButtonText(LPCSTR strCaption) { _buttonText = strCaption; }
    void MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                   SDL_Texture* pScene_background, SDL_Texture* pScreenTexture);
    void MouseUp(SDL_Event& event);
    void DrawButton(SDL_Surface* pScreen);
    void EnableWindow(bool bVal) { _enabled = bVal; }
    void SetVisibleState(VisbleState eVal);
    void RedrawButton(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                      SDL_Texture* pScreenTexture);
    void AddLineText(LPCSTR strLine);
    void SelectIndex(int iIndex);
    int GetSlectedIndex() { return _currDataIndex; }
    LPCSTR GetSelectedText() { return _vctDataStrings[_currDataIndex].c_str(); }
    int PosX() { return _rctCtrl.x; }
    int PosY() { return _rctCtrl.y; }
    int Height() { return _rctCtrl.h; }

private:
    VisbleState _visibleState;
    SDL_Rect _rctCtrl;
    SDL_Rect _rctText;
    SDL_Rect _rctBoxUp;
    SDL_Rect _rctBoxDown;
    STRING _buttonText;
    bool _enabled;
    ClickCb _fncbClickEvent;
    SDL_Renderer* _p_sdlRenderer;
    SDL_Surface* _p_surfBar;
    SDL_Surface* _p_surfBoxSel;
    SDL_Surface* _p_surfBoxUNSel;
    TTF_Font* _p_fontText;
    SDL_Color _color;
    int _butID;
    VCT_STRING _vctDataStrings;
    int _currDataIndex;
};

#endif
