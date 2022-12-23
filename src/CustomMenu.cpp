#include "CustomMenu.h"

#include "gfx_util.h"

static const char *lpszFontFile = DATA_PREFIX "images/font.bmp";

CustomMenu::CustomMenu() {
    _labelMenu = "Menu";
    _labelExit = "Exit";
    _p_BackgroundBitmap = NULL;
    _p_Screen = NULL;
}

CustomMenu::~CustomMenu() {
    if (_p_BackgroundBitmap != NULL)
        SDL_FreeSurface(_p_BackgroundBitmap);
    if (_p_ScreenTexture != NULL) {
        SDL_DestroyTexture(_p_ScreenTexture);
    }
    delete _p_Font;
}

LPErrInApp CustomMenu::Initialize(SDL_Surface *s, SDL_Renderer *r,
                                  std::string filenameBackground) {
    _p_Font = new CustomFont;
    _p_sdlRenderer = r;
    _p_Screen = s;
    _filenameBackground = filenameBackground;
    _p_ScreenTexture = SDL_CreateTexture(
        r, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s->w, s->h);
    if (_p_ScreenTexture == NULL) {
        return ERR_UTIL::ErrorCreate("Cannot create texture: %s\n",
                                     SDL_GetError());
    }

    return _p_Font->LoadFont(lpszFontFile);
}

void CustomMenu::AddItems(std::string item) { _vctItems.push_back(item); }

void CustomMenu::ClearItems() { _vctItems.clear(); }

LPErrInApp CustomMenu::Run(int &slectedItem) {
    TRACE("Menu Run with %d item(s)", _vctItems.size());
    bool bEnd = false;
    bool bRedraw = true;
    SDL_Rect trect;
    SDL_Event event;
    std::string name, temp;
    Uint32 tcolor;
    int counter = 0;
    slectedItem = -1;
    _vctItems.push_back(_labelExit);
    _iCurPos = 0;
    trect.x = _rect.x;
    trect.w = _rect.w;
    trect.h = SDLFONTSIZE;
    LPErrInApp err = LoadBackground();
    if (err != NULL)
        return err;
    while (bEnd == false) {
        if (bRedraw == true) {
            if (_filenameBackground.size() > 0) {
                err = drawBitmap(3, SDL_MapRGB(_p_Screen->format, 0, 0, 0));
                if (err != NULL)
                    return err;
            }

            SDL_FillRect(_p_Screen, &_rect, _colorBack);
            _p_Font->DrawString(_p_Screen, _labelMenu, TEXTMIXED,
                                TEXTALIGNCENTER, 0, _rect.y + 5, 0);

            for (unsigned int k = 0; k < _vctItems.size(); k++) {
                trect.y =
                    _rect.y + (25 + SDLFONTSIZE) + ((SDLFONTSIZE + 5) * k);

                if (k == _iCurPos) {
                    tcolor = _colorHighlight;
                } else {
                    tcolor = _colorBack;
                }

                SDL_FillRect(_p_Screen, &trect, tcolor);
                _p_Font->DrawString(
                    _p_Screen, _vctItems[k], TEXTMIXED, TEXTALIGNCENTER, 0,
                    _rect.y + (25 + SDLFONTSIZE) + ((SDLFONTSIZE + 5) * k), 0);
            }
            updateScreenTexture();
            bRedraw = false;
        }
        if (SDL_WaitEvent(&event) == 1) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            _iCurPos = -1;
                            bEnd = true;
                            break;
                        case SDLK_RETURN:
                            if (_iCurPos == _vctItems.size() - 1)
                                _iCurPos = -1;
                            bEnd = true;
                            break;
                        case SDLK_UP:
                            if (_iCurPos == 0)
                                _iCurPos = (signed int)_vctItems.size() - 1;
                            else
                                _iCurPos--;
                            bRedraw = true;
                            break;
                        case SDLK_DOWN:
                            if (_iCurPos == _vctItems.size() - 1)
                                _iCurPos = 0;
                            else
                                _iCurPos++;
                            bRedraw = true;
                            break;
                    };
                    break;
                case SDL_QUIT:
                    _iCurPos = -1;
                    bEnd = true;
                    break;
            }
        }
        SDL_Delay(50);
        counter++;
        if (counter % 10 == 0)
            bRedraw = true;
    }
    _vctItems.pop_back();
    slectedItem = _iCurPos;
    return NULL;
}

void CustomMenu::updateScreenTexture() {
    // SDL 2.0
    SDL_UpdateTexture(_p_ScreenTexture, NULL, _p_Screen->pixels,
                      _p_Screen->pitch);
    SDL_RenderClear(_p_sdlRenderer);
    SDL_RenderCopy(_p_sdlRenderer, _p_ScreenTexture, NULL, NULL);
    SDL_RenderPresent(_p_sdlRenderer);
}

void CustomMenu::SetLabels(std::string menu, std::string exit) {
    _labelMenu = menu;
    _labelExit = exit;
}

void CustomMenu::SetArea(int x, int y, int w, int h) {
    _rect.x = x;
    _rect.y = y;
    _rect.w = w;
    _rect.h = h;
}
void CustomMenu::SetColors(Uint32 back, Uint32 high) {
    _colorBack = back;
    _colorHighlight = high;
}

SDL_Rect *CustomMenu::GetArea() { return &_rect; }

LPErrInApp CustomMenu::LoadBackground() {
    if (_p_BackgroundBitmap != NULL) {
        SDL_FreeSurface(_p_BackgroundBitmap);
    }
    if (_filenameBackground.size() == 0) {
        return NULL;
    }

    _p_BackgroundBitmap = IMG_Load(_filenameBackground.c_str());
    if (_p_BackgroundBitmap == NULL) {
        return ERR_UTIL::ErrorCreate("Error IMG_Load: %s\n", SDL_GetError());
    }
    return NULL;
}

LPErrInApp CustomMenu::drawBitmap(unsigned char alignment, Uint32 color) {
    if (_p_BackgroundBitmap == NULL) {
        return NULL;  // No background bitmap provided, ignore draw
    }
    SDL_Rect s, t;

    // SDL_Surface *screen = SDL_GetVideoSurface();

    if (_p_Screen == NULL)
        return ERR_UTIL::ErrorCreate("Cannot draw bitmap: screen is invalid");

    s.w = _p_BackgroundBitmap->w;
    t.w = _p_BackgroundBitmap->w;
    s.h = _p_BackgroundBitmap->h;
    t.h = _p_BackgroundBitmap->h;
    s.x = 0;
    s.y = 0;
    switch (alignment) {
        case 0:  // LEFT
            t.x = 0;
            t.y = 0;
            break;
        case 1:  // CENTER
            t.x = (_p_Screen->w - _p_BackgroundBitmap->w) / 2;
            t.y = (_p_Screen->h - _p_BackgroundBitmap->h) / 2;
            break;
        case 2:  // RIGHT
            t.x = (_p_Screen->w - _p_BackgroundBitmap->w);
            t.y = (_p_Screen->h - _p_BackgroundBitmap->h);
            break;
        case 3:  // HORIZONTAL CENTER ONLY
            t.x = (_p_Screen->w - _p_BackgroundBitmap->w) / 2;
            t.y = 0;
            break;
        case 4:  // VERTICAL CENTER ONLY
            t.x = 0;
            t.y = (_p_Screen->h - _p_BackgroundBitmap->h) / 2;
            break;
    }

    SDL_FillRect(_p_Screen, &_p_Screen->clip_rect, color);

    while (SDL_BlitSurface(_p_BackgroundBitmap, &s, _p_Screen, &t) == -2)
        SDL_Delay(50);
    return NULL;
}

void CustomMenu::SetAlpha(unsigned char alpha) {
    SDL_SetSurfaceAlphaMod(_p_Screen, alpha);
}
