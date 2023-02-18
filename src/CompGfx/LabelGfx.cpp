#include "LabelGfx.h"

#include "GfxUtil.h"

LabelGfx::LabelGfx() {
    _StateGfx = INVISIBLE;
    _p_FontText = 0;
    _isEnabled = true;
    _p_Surf_Bar = 0;
}

LabelGfx::~LabelGfx() {
    if (_p_Surf_Bar) {
        SDL_FreeSurface(_p_Surf_Bar);
        _p_Surf_Bar = NULL;
    }
}

void LabelGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                          TTF_Font* pFont, SDL_Renderer* psdlRenderer) {
    _rctCtrl = *pRect;

    // black bar surface
    _p_Surf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, _rctCtrl.w, _rctCtrl.h,
                                       32, 0, 0, 0, 0);
    SDL_FillRect(_p_Surf_Bar, NULL, SDL_MapRGBA(pScreen->format, 255, 0, 0, 0));
    SDL_SetSurfaceBlendMode(_p_Surf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(_p_Surf_Bar, 127);  // SDL 2.0
    _p_FontText = pFont;

    _color = GFX_UTIL_COLOR::White;
    _p_sdlRenderer = psdlRenderer;
}

void LabelGfx::SetState(SateGfx eVal) {
    SateGfx eOldState = _StateGfx;
    _StateGfx = eVal;
    if (eOldState != _StateGfx && _StateGfx == VISIBLE) {
        _color = GFX_UTIL_COLOR::White;
    }
}

void LabelGfx::MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                         SDL_Texture* pScene_background,
                         SDL_Texture* pScreenTexture) {
    if (_StateGfx == VISIBLE && _isEnabled) {
        if (event.motion.x >= _rctCtrl.x &&
            event.motion.x <= _rctCtrl.x + _rctCtrl.w &&
            event.motion.y >= _rctCtrl.y &&
            event.motion.y <= _rctCtrl.y + _rctCtrl.h) {
            // mouse inner button
            _color = GFX_UTIL_COLOR::Orange;
            Redraw(pScreen, pScene_background, pScreenTexture);
        } else {
            // mouse outside
            if (_color.r == GFX_UTIL_COLOR::Orange.r &&
                _color.g == GFX_UTIL_COLOR::Orange.g &&
                _color.b == GFX_UTIL_COLOR::Orange.b) {
                // button was selected
                _color = GFX_UTIL_COLOR::White;
                Redraw(pScreen, pScene_background, pScreenTexture);
            }
        }
    }
}

void LabelGfx::MouseUp(SDL_Event& event) {}

void LabelGfx::Draw(SDL_Surface* pScreen) {
    if (_StateGfx != INVISIBLE) {
        if (_isEnabled) {
            int tx, ty;
            TTF_SizeText(_p_FontText, _CtrlText.c_str(), &tx, &ty);
            int iXOffSet = (_rctCtrl.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            int iYOffset = (_rctCtrl.h - ty) / 2;

            iYOffset = 0;
            iXOffSet = 0;
            GFX_UTIL::DrawString(pScreen, _CtrlText.c_str(),
                                 _rctCtrl.x + iXOffSet, _rctCtrl.y + iYOffset,
                                 _color, _p_FontText, false);
        }
    }
}

void LabelGfx::Redraw(SDL_Surface* pScreen, SDL_Texture* pScene_background,
                      SDL_Texture* pScreenTexture) {
    if (pScene_background) {
        SDL_RenderCopy(_p_sdlRenderer, pScene_background, &_rctCtrl,
                       &_rctCtrl);  // SDL 2.0
    }
    Draw(pScreen);
    SDL_UpdateTexture(pScreenTexture, NULL, pScreen->pixels, pScreen->pitch);
    SDL_RenderCopy(_p_sdlRenderer, pScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}
