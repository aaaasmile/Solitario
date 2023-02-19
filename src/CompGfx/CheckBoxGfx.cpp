#include "CheckBoxGfx.h"

#include "GfxUtil.h"

CheckBoxGfx::CheckBoxGfx() {
    _visibleState = INVISIBLE;
    _p_FontText = 0;
    _enabled = true;
    _buttonID = -1;
}

CheckBoxGfx::~CheckBoxGfx() {}

void CheckBoxGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                             TTF_Font* pFont, int iButID,
                             CheckboxClickCb& fncbClickEvent) {
    _fncbClickEvent = fncbClickEvent;
    _rctCtrl = *pRect;

    _p_FontText = pFont;

    _color = GFX_UTIL_COLOR::White;
    _colorBorder = _color;

    _clicked = false;

    _buttonID = iButID;
}

void CheckBoxGfx::SetVisibleState(VisbleState eVal) {
    VisbleState eOldState = _visibleState;
    _visibleState = eVal;
    if (eOldState != _visibleState && _visibleState == VISIBLE) {
        _color = GFX_UTIL_COLOR::White;
    }
}

void CheckBoxGfx::MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                            SDL_Surface* pScene_background) {}

void CheckBoxGfx::MouseUp(SDL_Event& event) {
    if (_visibleState == VISIBLE && _enabled) {
        if (event.motion.x >= _rctCtrl.x &&
            event.motion.x <= _rctCtrl.x + _rctCtrl.w &&
            event.motion.y >= _rctCtrl.y &&
            event.motion.y <= _rctCtrl.y + _rctCtrl.h) {
            if (_clicked) {
                _clicked = false;
            } else {
                _clicked = true;
            }
            if ((_fncbClickEvent.tc) != NULL)
                (_fncbClickEvent.tc)->Click(_fncbClickEvent.self, _clicked);
        }
    }
}

void CheckBoxGfx::DrawButton(SDL_Surface* pScreen) {
    if (_visibleState != INVISIBLE) {
        if (_enabled) {
            int iXOffSet = 0;
            int iYOffset = 0;

            if (_clicked) {
                SDL_Rect rctCheck;
                rctCheck.x = _rctCtrl.x;
                rctCheck.y = _rctCtrl.y;
                rctCheck.w = CHECK_W;
                rctCheck.h = CHECK_H;

                GFX_UTIL::DrawRect(pScreen, rctCheck.x - 1, rctCheck.y - 1,
                                   rctCheck.x + rctCheck.w + 1,
                                   rctCheck.y + rctCheck.h + 1,
                                   GFX_UTIL_COLOR::Gray);
                GFX_UTIL::DrawRect(pScreen, rctCheck.x - 2, rctCheck.y - 2,
                                   rctCheck.x + rctCheck.w + 2,
                                   rctCheck.y + rctCheck.h + 2,
                                   GFX_UTIL_COLOR::Black);
                GFX_UTIL::DrawStaticLine(
                    pScreen, rctCheck.x, rctCheck.y, rctCheck.x + rctCheck.w,
                    rctCheck.y + rctCheck.h, GFX_UTIL_COLOR::Orange);
                GFX_UTIL::DrawStaticLine(
                    pScreen, rctCheck.x + rctCheck.w, rctCheck.y, rctCheck.x,
                    rctCheck.y + rctCheck.h, GFX_UTIL_COLOR::Orange);

                iXOffSet = rctCheck.w + 10;
                iYOffset = rctCheck.h;

            } else {
                SDL_Rect rctCheck;
                rctCheck.x = _rctCtrl.x;
                rctCheck.y = _rctCtrl.y;
                rctCheck.w = CHECK_W;
                rctCheck.h = CHECK_H;

                GFX_UTIL::DrawRect(pScreen, rctCheck.x - 1, rctCheck.y - 1,
                                   rctCheck.x + rctCheck.w + 1,
                                   rctCheck.y + rctCheck.h + 1,
                                   GFX_UTIL_COLOR::Gray);
                GFX_UTIL::DrawRect(pScreen, rctCheck.x - 2, rctCheck.y - 2,
                                   rctCheck.x + rctCheck.w + 2,
                                   rctCheck.y + rctCheck.h + 2,
                                   GFX_UTIL_COLOR::Black);

                iXOffSet = rctCheck.w + 10;
                iYOffset = rctCheck.h;
            }

            int tx, ty;
            TTF_SizeText(_p_FontText, _buttonText.c_str(), &tx, &ty);

            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            GFX_UTIL::DrawString(
                pScreen, _buttonText.c_str(), _rctCtrl.x + iXOffSet,
                _rctCtrl.y + iYOffset - ty, _color, _p_FontText);
        }
    }
}

void CheckBoxGfx::RedrawButton(SDL_Surface* pScreen,
                               SDL_Surface* pScene_background) {
    if (pScene_background) {
        SDL_BlitSurface(pScene_background, &_rctCtrl, pScreen, &_rctCtrl);
    }
    DrawButton(pScreen);
}
