#include "ComboGfx.h"

#include "GfxUtil.h"

static const char* LP_PLUS = "+";
static const char* LP_MINUS = "-";

ComboGfx::ComboGfx() {
    m_eState = INVISIBLE;
    m_pFontText = 0;
    m_iButID = 0;
    m_bIsEnabled = true;
    m_pSurf_Bar = 0;
    m_iCurrDataIndex = 0;
    m_pSurf_BoxSel = 0;
    m_pSurf_BoxUNSel = 0;
}

ComboGfx::~ComboGfx() {
    if (m_pSurf_Bar) {
        SDL_FreeSurface(m_pSurf_Bar);
        m_pSurf_Bar = NULL;
    }
    if (m_pSurf_BoxSel) {
        SDL_FreeSurface(m_pSurf_BoxSel);
        m_pSurf_BoxSel = NULL;
    }
    if (m_pSurf_BoxUNSel) {
        SDL_FreeSurface(m_pSurf_BoxUNSel);
        m_pSurf_BoxUNSel = NULL;
    }
}

void ComboGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                          TTF_Font* pFont, int iButID,
                          SDL_Renderer* psdlRenderer, ClickCb& fncbClickEvent) {
    m_fncbClickEvent = fncbClickEvent;
    m_rctButt = *pRect;
    m_psdlRenderer = psdlRenderer;

    int iWBoxInc = 20;
    int iHBoxInc = m_rctButt.h / 2;
    m_rctText.x = m_rctButt.x;
    m_rctText.y = m_rctButt.y;
    m_rctText.w = m_rctButt.w - iWBoxInc;
    m_rctText.h = m_rctButt.h;

    m_rctBoxUp.x = m_rctText.x + m_rctText.w;
    m_rctBoxUp.y = m_rctText.y;
    m_rctBoxUp.w = iWBoxInc;
    m_rctBoxUp.h = iHBoxInc;

    m_rctBoxDown.x = m_rctBoxUp.x;
    m_rctBoxDown.y = m_rctBoxUp.y + iHBoxInc;
    m_rctBoxDown.w = iWBoxInc;
    m_rctBoxDown.h = iHBoxInc;

    m_pSurf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctButt.w, m_rctButt.h,
                                       32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_Bar, NULL,
                 SDL_MapRGBA(pScreen->format, 255, 128, 30, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_Bar, 127);

    m_pSurf_BoxSel = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctBoxUp.w,
                                          m_rctBoxUp.h, 32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_BoxSel, NULL,
                 SDL_MapRGBA(pScreen->format, 200, 200, 130, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_BoxSel, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_BoxSel, 127);

    m_pSurf_BoxUNSel = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctBoxUp.w,
                                            m_rctBoxUp.h, 32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_BoxUNSel, NULL,
                 SDL_MapRGBA(pScreen->format, 255, 128, 30, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_BoxUNSel, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_BoxUNSel, 127);

    m_pFontText = pFont;

    m_colCurrent = GFX_UTIL_COLOR::White;
    m_iButID = iButID;
    m_iCurrDataIndex = 0;
    m_vctDataStrings.clear();
}

void ComboGfx::AddLineText(LPCSTR strLine) {
    m_vctDataStrings.push_back(strLine);
}

void ComboGfx::SelectIndex(int iIndex) {
    if (iIndex >= 0 && iIndex < m_vctDataStrings.size()) {
        m_iCurrDataIndex = iIndex;
    }
}

void ComboGfx::SetState(eSate eVal) {
    eSate eOldState = m_eState;
    m_eState = eVal;
    if (eOldState != m_eState && m_eState == VISIBLE) {
        m_colCurrent = GFX_UTIL_COLOR::White;
    }
}

void ComboGfx::MouseMove(SDL_Event& event, SDL_Surface* pScreen,
                         SDL_Texture* pScene_background,
                         SDL_Texture* pScreenTexture) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            // mouse inner button
            m_colCurrent = GFX_UTIL_COLOR::Orange;
            RedrawButton(pScreen, pScene_background, pScreenTexture);
        } else {
            // mouse outside
            if (m_colCurrent.r == GFX_UTIL_COLOR::Orange.r &&
                m_colCurrent.g == GFX_UTIL_COLOR::Orange.g &&
                m_colCurrent.b == GFX_UTIL_COLOR::Orange.b) {
                // button was selected
                m_colCurrent = GFX_UTIL_COLOR::White;
                RedrawButton(pScreen, pScene_background, pScreenTexture);
            }
        }
    }
}

void ComboGfx::MouseUp(SDL_Event& event) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        int mx = event.motion.x;
        int my = event.motion.y;
        if (mx >= m_rctBoxUp.x && mx <= m_rctBoxUp.x + m_rctBoxUp.w &&
            my >= m_rctBoxUp.y && my <= m_rctBoxUp.y + m_rctBoxUp.h) {
            // mouse on up box
            m_iCurrDataIndex++;
            if (m_iCurrDataIndex >= m_vctDataStrings.size()) {
                // circular buffer
                m_iCurrDataIndex = 0;
            }
            if ((m_fncbClickEvent.tc) != NULL)
                (m_fncbClickEvent.tc)
                    ->Click(m_fncbClickEvent.self, m_iCurrDataIndex);

        } else if (mx >= m_rctBoxDown.x &&
                   mx <= m_rctBoxDown.x + m_rctBoxDown.w &&
                   my >= m_rctBoxDown.y &&
                   my <= m_rctBoxDown.y + m_rctBoxDown.h) {
            // mouse on down box
            if (m_iCurrDataIndex == 0) {
                // circular buffer
                m_iCurrDataIndex = m_vctDataStrings.size() - 1;
            } else {
                m_iCurrDataIndex--;
            }
            if ((m_fncbClickEvent.tc) != NULL)
                (m_fncbClickEvent.tc)
                    ->Click(m_fncbClickEvent.self, m_iCurrDataIndex);
        }
    }
}

void ComboGfx::DrawButton(SDL_Surface* pScreen) {
    if (m_eState != INVISIBLE) {
        bool bUpBoxSel = false;
        bool bDownBoxSel = false;
        if (m_bIsEnabled) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (mx >= m_rctBoxUp.x && mx <= m_rctBoxUp.x + m_rctBoxUp.w &&
                my >= m_rctBoxUp.y && my <= m_rctBoxUp.y + m_rctBoxUp.h) {
                // mouse on up box
                bUpBoxSel = true;
            } else if (mx >= m_rctBoxDown.x &&
                       mx <= m_rctBoxDown.x + m_rctBoxDown.w &&
                       my >= m_rctBoxDown.y &&
                       my <= m_rctBoxDown.y + m_rctBoxDown.h) {
                // mouse on down box
                bDownBoxSel = true;
            }

            // background on up/down boxes
            if (bUpBoxSel) {
                GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctBoxUp.w,
                                             m_rctBoxUp.h, m_rctBoxUp.x,
                                             m_rctBoxUp.y, m_pSurf_BoxSel);
            } else {
                GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctBoxUp.w,
                                             m_rctBoxUp.h, m_rctBoxUp.x,
                                             m_rctBoxUp.y, m_pSurf_BoxUNSel);
            }
            if (bDownBoxSel) {
                GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctBoxDown.w,
                                             m_rctBoxDown.h, m_rctBoxDown.x,
                                             m_rctBoxDown.y, m_pSurf_BoxSel);
            } else {
                GFX_UTIL::DrawStaticSpriteEx(pScreen, 0, 0, m_rctBoxDown.w,
                                             m_rctBoxDown.h, m_rctBoxDown.x,
                                             m_rctBoxDown.y, m_pSurf_BoxUNSel);
            }

            // draw current selected text
            int tx, ty;
            TTF_SizeText(m_pFontText, m_strButText.c_str(), &tx, &ty);
            int iXOffSet = (m_rctText.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            int iYOffset = (m_rctText.h - ty) / 2;
            m_strButText = m_vctDataStrings[m_iCurrDataIndex];
            GFX_UTIL::DrawString(pScreen, m_strButText.c_str(),
                                 m_rctText.x + iXOffSet, m_rctText.y + iYOffset,
                                 GFX_UTIL_COLOR::White, m_pFontText, false);

            // draw text upper box
            TTF_SizeText(m_pFontText, LP_PLUS, &tx, &ty);
            iXOffSet = (m_rctBoxUp.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            iYOffset = 0;
            if (bUpBoxSel) {
                m_colCurrent = GFX_UTIL_COLOR::Orange;
            } else {
                m_colCurrent = GFX_UTIL_COLOR::White;
            }
            GFX_UTIL::DrawString(pScreen, LP_PLUS, m_rctBoxUp.x + iXOffSet,
                                 m_rctBoxUp.y + iYOffset, m_colCurrent,
                                 m_pFontText, false);

            // draw text down box
            TTF_SizeText(m_pFontText, LP_MINUS, &tx, &ty);
            iXOffSet = (m_rctBoxDown.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            if (bDownBoxSel) {
                m_colCurrent = GFX_UTIL_COLOR::Orange;
            } else {
                m_colCurrent = GFX_UTIL_COLOR::White;
            }
            GFX_UTIL::DrawString(pScreen, LP_MINUS, m_rctBoxDown.x + iXOffSet,
                                 m_rctBoxDown.y + iYOffset, m_colCurrent,
                                 m_pFontText, false);

            // draw borders
            GFX_UTIL::DrawRect(pScreen, m_rctButt.x - 1, m_rctButt.y - 1,
                               m_rctButt.x + m_rctButt.w + 1,
                               m_rctButt.y + m_rctButt.h + 1,
                               GFX_UTIL_COLOR::Gray);
            GFX_UTIL::DrawRect(pScreen, m_rctButt.x - 2, m_rctButt.y - 2,
                               m_rctButt.x + m_rctButt.w + 2,
                               m_rctButt.y + m_rctButt.h + 2,
                               GFX_UTIL_COLOR::Black);
            GFX_UTIL::DrawRect(
                pScreen, m_rctButt.x, m_rctButt.y, m_rctButt.x + m_rctButt.w,
                m_rctButt.y + m_rctButt.h, GFX_UTIL_COLOR::White);

            GFX_UTIL::DrawRect(pScreen, m_rctBoxUp.x + 1, m_rctBoxUp.y + 1,
                               m_rctBoxUp.x + m_rctBoxUp.w - 1,
                               m_rctBoxUp.y + m_rctBoxUp.h - 1,
                               GFX_UTIL_COLOR::Black);

            GFX_UTIL::DrawRect(pScreen, m_rctBoxDown.x + 1, m_rctBoxDown.y + 1,
                               m_rctBoxDown.x + m_rctBoxDown.w - 1,
                               m_rctBoxDown.y + m_rctBoxDown.h - 1,
                               GFX_UTIL_COLOR::Black);

            GFX_UTIL::DrawRect(pScreen, m_rctBoxDown.x, m_rctBoxUp.y,
                               m_rctBoxDown.x + m_rctBoxDown.w,
                               m_rctBoxUp.y + m_rctBoxUp.h + m_rctBoxDown.h,
                               GFX_UTIL_COLOR::White);
        }
    }
}

void ComboGfx::RedrawButton(SDL_Surface* pScreen,
                            SDL_Texture* pScene_background,
                            SDL_Texture* pScreenTexture) {
    if (pScene_background) {
        SDL_RenderCopy(m_psdlRenderer, pScene_background, NULL, NULL);
    }
    DrawButton(pScreen);
    SDL_RenderCopy(m_psdlRenderer, pScreenTexture, NULL, NULL);
    SDL_RenderPresent(m_psdlRenderer);
}
