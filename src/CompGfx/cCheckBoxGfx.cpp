#include "cCheckBoxGfx.h"

#include "GfxUtil.h"

cCheckBoxGfx::cCheckBoxGfx() {
    m_eState = INVISIBLE;
    m_pFontText = 0;
    m_bIsEnabled = true;
    m_iButID = -1;
}

cCheckBoxGfx::~cCheckBoxGfx() {}

void cCheckBoxGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                              TTF_Font* pFont, int iButID,
                              CheckboxClickCb& fncbClickEvent) {
    m_fncbClickEvent = fncbClickEvent;
    m_rctButt = *pRect;

    m_pFontText = pFont;

    m_colCurrent = GFX_UTIL_COLOR::White;
    m_colBorder = m_colCurrent;

    m_bClickState = false;

    m_iButID = iButID;
}

void cCheckBoxGfx::SetState(eSate eVal) {
    eSate eOldState = m_eState;
    m_eState = eVal;
    if (eOldState != m_eState && m_eState == VISIBLE) {
        m_colCurrent = GFX_UTIL_COLOR::White;
    }
}

void cCheckBoxGfx::MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                             SDL_Surface* pScene_background) {}

void cCheckBoxGfx::MouseUp(SDL_Event& event) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            if (m_bClickState) {
                m_bClickState = false;
            } else {
                m_bClickState = true;
            }
            if ((m_fncbClickEvent.tc) != NULL)
                (m_fncbClickEvent.tc)
                    ->Click(m_fncbClickEvent.self, m_bClickState);
        }
    }
}

void cCheckBoxGfx::DrawButton(SDL_Surface* pScreen) {
    if (m_eState != INVISIBLE) {
        if (m_bIsEnabled) {
            int iXOffSet = 0;
            int iYOffset = 0;

            if (m_bClickState) {
                SDL_Rect rctCheck;
                rctCheck.x = m_rctButt.x;
                rctCheck.y = m_rctButt.y;
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
                rctCheck.x = m_rctButt.x;
                rctCheck.y = m_rctButt.y;
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
            TTF_SizeText(m_pFontText, m_strButText.c_str(), &tx, &ty);

            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            GFX_UTIL::DrawString(
                pScreen, m_strButText.c_str(), m_rctButt.x + iXOffSet,
                m_rctButt.y + iYOffset - ty, m_colCurrent, m_pFontText, false);
        }
    }
}

void cCheckBoxGfx::RedrawButton(SDL_Surface* pScreen,
                                SDL_Surface* pScene_background) {
    if (pScene_background) {
        SDL_BlitSurface(pScene_background, &m_rctButt, pScreen, &m_rctButt);
    }
    DrawButton(pScreen);
}
