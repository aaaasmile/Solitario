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
    delete _p_Font;
}

LPErrInApp CustomMenu::Initialize() {
    _p_Font = new CustomFont;
    return _p_Font->LoadFont(lpszFontFile);
}

void CustomMenu::AddItems(string item) { _vctItems.push_back(item); }

void CustomMenu::ClearItems() { _vctItems.clear(); }

LPErrInApp CustomMenu::Run(int &slectedItem) {
    TRACE("Menu Run with %d item(s)", _vctItems.size());
    bool bEnd = false;
    bool bRedraw = true;
    SDL_Rect trect;
    SDL_Event event;
    string name, temp;
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
    SDL_Surface *surface;  // TODO SDL 1.2
    while (bEnd == false) {
        if (bRedraw == true) {
            if (FilenameBackground.size() > 0) {
                err = DrawBitmap(3, SDL_MapRGB(_p_Screen->format, 0, 0, 0),
                                 surface);
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
            // SDL_Flip(_p_Screen); // TODO SDL 1.2
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

void CustomMenu::SetLabels(string menu, string exit) {
    _labelMenu = menu;
    _labelExit = exit;
}

void CustomMenu::SetBitmap(string filename) { FilenameBackground = filename; }
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
    if (FilenameBackground.size() > 0) {
        return NULL;
    }

    _p_BackgroundBitmap = IMG_Load(FilenameBackground.c_str());
    if (_p_BackgroundBitmap == NULL) {
        return ERR_UTIL::ErrorCreate("Error IMG_Load: %s\n", SDL_GetError());
    }
    return NULL;
}

LPErrInApp CustomMenu::DrawBitmap(unsigned char alignment, Uint32 color,
                                  SDL_Surface *screen) {
    if (_p_BackgroundBitmap == NULL) {
        return NULL;  // No background bitmap provided, ignore draw
    }
    SDL_Rect s, t;

    // SDL_Surface *screen = SDL_GetVideoSurface();

    if (screen == NULL)
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
            t.x = (screen->w - _p_BackgroundBitmap->w) / 2;
            t.y = (screen->h - _p_BackgroundBitmap->h) / 2;
            break;
        case 2:  // RIGHT
            t.x = (screen->w - _p_BackgroundBitmap->w);
            t.y = (screen->h - _p_BackgroundBitmap->h);
            break;
        case 3:  // HORIZONTAL CENTER ONLY
            t.x = (screen->w - _p_BackgroundBitmap->w) / 2;
            t.y = 0;
            break;
        case 4:  // VERTICAL CENTER ONLY
            t.x = 0;
            t.y = (screen->h - _p_BackgroundBitmap->h) / 2;
            break;
    }
    //   if (Bitmap->format->palette && screen->format->palette)
    //     SDL_SetColors(screen, Bitmap->format->palette->colors, 0,
    //     TODO SDL 1.2 SDL 2.0
    //                   Bitmap->format->palette->ncolors);

    SDL_FillRect(screen, &screen->clip_rect, color);

    while (SDL_BlitSurface(_p_BackgroundBitmap, &s, screen, &t) == -2)
        SDL_Delay(50);
    return NULL;
}

void CustomMenu::SetAlpha(unsigned char alpha) {
    SDL_SetSurfaceAlphaMod(_p_Screen, alpha);
}
