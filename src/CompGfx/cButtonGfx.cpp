#include "cButtonGfx.h"

#include "GfxUtil.h"

cButtonGfx::cButtonGfx() {
    m_eState = INVISIBLE;
    m_pFontText = 0;
    m_iButID = 0;
    m_bIsEnabled = true;
    m_pSurf_Bar = 0;
    m_bMouseIsDown = false;
    m_eButType = TEXT_BUTTON;
    m_pSurf_Bitmap_Act1 = 0;
    m_pSurf_Bitmap_MOv2 = 0;
    m_pSurf_Bitmap_Pus3 = 0;
}

cButtonGfx::~cButtonGfx() {
    if (m_pSurf_Bar) {
        SDL_FreeSurface(m_pSurf_Bar);
        m_pSurf_Bar = NULL;
    }
}

void cButtonGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                            TTF_Font* pFont, int iButID,
                            SDL_Renderer* psdlRenderer,
                            ClickCb& fncbClickEvent) {
    m_fncbClickEvent = fncbClickEvent;
    m_rctButt = *pRect;

    m_pSurf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctButt.w, m_rctButt.h,
                                       32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_Bar, NULL, SDL_MapRGBA(pScreen->format, 255, 0, 0, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_Bar, 127);  // SDL 2.0
    m_pFontText = pFont;
    m_colCurrent = GFX_UTIL_COLOR::White;
    m_iButID = iButID;
    m_bMouseIsDown = false;
    m_eButType = TEXT_BUTTON;
    m_psdlRenderer = psdlRenderer;
}

void cButtonGfx::SetState(eSate eVal) {
    eSate eOldState = m_eState;
    m_eState = eVal;
    if (eOldState != m_eState && m_eState == VISIBLE) {
        m_colCurrent = GFX_UTIL_COLOR::White;
    }
}

void cButtonGfx::MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                           SDL_Surface* pScene_background,
                           SDL_Texture* pScreenTexture) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            m_colCurrent = GFX_UTIL_COLOR::Orange;
            RedrawButton(pScreen, pScene_background, pScreenTexture);
        } else {
            if (m_colCurrent.r == GFX_UTIL_COLOR::Orange.r &&
                m_colCurrent.g == GFX_UTIL_COLOR::Orange.g &&
                m_colCurrent.b == GFX_UTIL_COLOR::Orange.b) {
                m_colCurrent = GFX_UTIL_COLOR::White;
                RedrawButton(pScreen, pScene_background, pScreenTexture);
            }
            m_bMouseIsDown = false;
        }
    }
}

void cButtonGfx::MouseDown(SDL_Event& event, SDL_Surface* pScreen,
                           SDL_Surface* pScene_background,
                           SDL_Texture* pScreenTexture) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            m_colCurrent = GFX_UTIL_COLOR::Orange;
            m_bMouseIsDown = true;
            RedrawButton(pScreen, pScene_background, pScreenTexture);
        }
    }
}

void cButtonGfx::MouseUp(SDL_Event& event) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            if ((m_fncbClickEvent.tc) != NULL)
                (m_fncbClickEvent.tc)->Click(m_fncbClickEvent.self, m_iButID);
        }
    }
    m_bMouseIsDown = false;
}

void cButtonGfx::DrawButton(SDL_Surface* pScreen) {
    if (m_eState != INVISIBLE) {
        if (m_bIsEnabled) {
            if (m_eButType == BITMAP_BUTTON) {
                drawBtAsBitmap(pScreen);
                return;
            }
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (mx >= m_rctButt.x && mx <= m_rctButt.x + m_rctButt.w &&
                my >= m_rctButt.y && my <= m_rctButt.y + m_rctButt.h) {
                m_colCurrent = GFX_UTIL_COLOR::Orange;
            } else {
                m_colCurrent = GFX_UTIL_COLOR::White;
            }

            GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctButt.w,
                                         m_rctButt.h, m_rctButt.x, m_rctButt.y,
                                         m_pSurf_Bar);
            int tx, ty;
            TTF_SizeText(m_pFontText, m_strButText.c_str(), &tx, &ty);
            int iXOffSet = (m_rctButt.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            int iYOffset = (m_rctButt.h - ty) / 2;
            GFX_UTIL::DrawString(pScreen, m_strButText.c_str(),
                                 m_rctButt.x + iXOffSet, m_rctButt.y + iYOffset,
                                 m_colCurrent, m_pFontText, false);

            // draw border
            GFX_UTIL::DrawRect(pScreen, m_rctButt.x - 1, m_rctButt.y - 1,
                               m_rctButt.x + m_rctButt.w + 1,
                               m_rctButt.y + m_rctButt.h + 1,
                               GFX_UTIL_COLOR::Gray);
            GFX_UTIL::DrawRect(pScreen, m_rctButt.x - 2, m_rctButt.y - 2,
                               m_rctButt.x + m_rctButt.w + 2,
                               m_rctButt.y + m_rctButt.h + 2,
                               GFX_UTIL_COLOR::Black);
            GFX_UTIL::DrawRect(pScreen, m_rctButt.x, m_rctButt.y,
                               m_rctButt.x + m_rctButt.w,
                               m_rctButt.y + m_rctButt.h, m_colCurrent);
        }
    }
}

void cButtonGfx::drawBtAsBitmap(SDL_Surface* pScreen) {
    SDL_Surface* pSurfToDraw = 0;
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    if (mx >= m_rctButt.x && mx <= m_rctButt.x + m_rctButt.w &&
        my >= m_rctButt.y && my <= m_rctButt.y + m_rctButt.h) {
        pSurfToDraw = m_pSurf_Bitmap_MOv2;

    } else {
        pSurfToDraw = m_pSurf_Bitmap_Act1;
    }
    if (m_bMouseIsDown) {
        pSurfToDraw = m_pSurf_Bitmap_Pus3;
    }
    SDL_SetSurfaceBlendMode(pSurfToDraw, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(pSurfToDraw, 180);  // SDL 2.0
    GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctButt.w, m_rctButt.h,
                                 m_rctButt.x, m_rctButt.y, pSurfToDraw);
}

void cButtonGfx::RedrawButton(SDL_Surface* pScreen,
                              SDL_Surface* pScene_background,
                              SDL_Texture* pScreenTexture) {
    if (pScene_background) {
        SDL_BlitSurface(pScene_background, &m_rctButt, pScreen, &m_rctButt);
    }
    DrawButton(pScreen);
    SDL_UpdateTexture(pScreenTexture, NULL, pScreen->pixels, pScreen->pitch);
    SDL_RenderCopy(m_psdlRenderer, pScreenTexture, NULL, NULL);
    SDL_RenderPresent(m_psdlRenderer);
}
