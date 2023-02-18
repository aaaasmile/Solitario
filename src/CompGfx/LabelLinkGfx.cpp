#include "LabelLinkGfx.h"

#include "GfxUtil.h"

LabelLinkGfx::LabelLinkGfx() {
    m_eState = INVISIBLE;
    m_pFontText = 0;
    m_iButID = 0;
    m_bIsEnabled = true;
    m_pSurf_Bar = 0;
    _isDirty = true;
}

LabelLinkGfx::~LabelLinkGfx() {
    if (m_pSurf_Bar) {
        SDL_FreeSurface(m_pSurf_Bar);
        m_pSurf_Bar = NULL;
    }
}

void LabelLinkGfx::Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                              TTF_Font* pFont, int iButID,
                              ClickCb& fncbClickEvent) {
    m_fncbClickEvent = fncbClickEvent;
    m_rctButt = *pRect;

    // black bar surface
    m_pSurf_Bar = SDL_CreateRGBSurface(SDL_SWSURFACE, m_rctButt.w, m_rctButt.h,
                                       32, 0, 0, 0, 0);
    SDL_FillRect(m_pSurf_Bar, NULL, SDL_MapRGBA(pScreen->format, 255, 0, 0, 0));
    SDL_SetSurfaceBlendMode(m_pSurf_Bar, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceAlphaMod(m_pSurf_Bar, 127);  // SDL 2.0

    m_pFontText = pFont;

    m_colCurrent = GFX_UTIL_COLOR::White;
    m_iButID = iButID;
}

void LabelLinkGfx::SetState(eSate eVal) {
    eSate eOldState = m_eState;
    m_eState = eVal;
    if (eOldState != m_eState && m_eState == VISIBLE) {
        m_colCurrent = GFX_UTIL_COLOR::White;
    }
    _isDirty = true;
}

void LabelLinkGfx::MouseMove(SDL_Event& event) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            // mouse inner button
            if (_mouseOuside) {
                _isDirty = true;
                _mouseOuside = false;
            }
        } else {
            // mouse outside
            if (!_mouseOuside) {
                _isDirty = true;
                _mouseOuside = true;
            }
        }
    }
}

void LabelLinkGfx::MouseUp(SDL_Event& event) {
    if (m_eState == VISIBLE && m_bIsEnabled) {
        if (event.motion.x >= m_rctButt.x &&
            event.motion.x <= m_rctButt.x + m_rctButt.w &&
            event.motion.y >= m_rctButt.y &&
            event.motion.y <= m_rctButt.y + m_rctButt.h) {
            // mouse go up inner the button
            // send a click event
            if ((m_fncbClickEvent.tc) != NULL)
                (m_fncbClickEvent.tc)->Click(m_fncbClickEvent.self, m_iButID);
            if (m_strUrl.length()) {
#ifdef WIN32
                const char* cmd = NULL;
                char cmdpath[PATH_MAX];

                cmd = "start";
                snprintf(cmdpath, sizeof(cmdpath), "%s %s", cmd,
                         m_strUrl.c_str());
                system(cmdpath);
#endif
            }
        }
    }
}

void LabelLinkGfx::Draw(SDL_Surface* pScreen) {
    if (!_isDirty) {
        return;
    }
    if (m_eState != INVISIBLE) {
        if (m_bIsEnabled) {
            // begin stuff mouse
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (mx >= m_rctButt.x && mx <= m_rctButt.x + m_rctButt.w &&
                my >= m_rctButt.y && my <= m_rctButt.y + m_rctButt.h) {
                // mouse on button
                m_colCurrent = GFX_UTIL_COLOR::Orange;
            } else {
                m_colCurrent = GFX_UTIL_COLOR::White;
            }
            // end stuff mouse

            int tx, ty;
            TTF_SizeText(m_pFontText, m_strButText.c_str(), &tx, &ty);
            int iXOffSet = (m_rctButt.w - tx) / 2;
            if (iXOffSet < 0) {
                iXOffSet = 1;
            }
            int iYOffset = (m_rctButt.h - ty) / 2;

            iYOffset = 0;
            iXOffSet = 0;
            GFX_UTIL::DrawString(pScreen, m_strButText.c_str(),
                                 m_rctButt.x + iXOffSet, m_rctButt.y + iYOffset,
                                 m_colCurrent, m_pFontText, false);
            _isDirty = false;
        }
    }
}
