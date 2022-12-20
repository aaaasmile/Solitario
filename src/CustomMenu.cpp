#include "CustomMenu.h"

#include "gfx_util.h"

static const char *lpszFontFile = DATA_PREFIX "images/font.bmp";

CustomMenu::CustomMenu() {
    LabelMenu = "Menu";
    LabelExit = "Exit";
    Bitmap = NULL;
    Font = new CustomFont;
    Font->LoadFont(lpszFontFile);
    Screen = NULL;
}
CustomMenu::~CustomMenu() {
    if (Bitmap != NULL)
        SDL_FreeSurface(Bitmap);
    delete Font;
}

void CustomMenu::AddItems(string item) { Items.push_back(item); }

void CustomMenu::ClearItems() { Items.clear(); }

////////////////////////////////////////
//       Run
/*! Run menu
 */
int CustomMenu::Run() {
    bool bEnd = false;
    bool bRedraw = true;
    SDL_Rect trect;
    SDL_Event event;
    string name, temp;
    Uint32 tcolor;
    int counter = 0;
    Items.push_back(LabelExit);
    iCurPos = 0;
    trect.x = Rect.x;
    trect.w = Rect.w;
    trect.h = SDLFONTSIZE;
    LoadBackground();
    SDL_Surface *surface;  // TODO SDL 2.0
    while (bEnd == false) {
        if (bRedraw == true) {
            if (FilenameBackground.size() > 0) {
                DrawBitmap(3, SDL_MapRGB(Screen->format, 0, 0, 0), surface);
            }

            SDL_FillRect(Screen, &Rect, ColorBack);
            Font->DrawString(Screen, LabelMenu, TEXTMIXED, TEXTALIGNCENTER, 0,
                             Rect.y + 5, 0);

            for (unsigned int k = 0; k < Items.size(); k++) {
                trect.y = Rect.y + (25 + SDLFONTSIZE) + ((SDLFONTSIZE + 5) * k);

                if (k == iCurPos) {
                    tcolor = ColorHighlight;
                } else {
                    tcolor = ColorBack;
                }

                SDL_FillRect(Screen, &trect, tcolor);
                Font->DrawString(
                    Screen, Items[k], TEXTMIXED, TEXTALIGNCENTER, 0,
                    Rect.y + (25 + SDLFONTSIZE) + ((SDLFONTSIZE + 5) * k), 0);
            }
            // SDL_Flip(Screen); // TODO SDL 2.0
            bRedraw = false;
        }
        if (SDL_WaitEvent(&event) == 1) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            iCurPos = -1;
                            bEnd = true;
                            break;
                        case SDLK_RETURN:
                            if (iCurPos == Items.size() - 1)
                                iCurPos = -1;
                            bEnd = true;
                            break;
                        case SDLK_UP:
                            if (iCurPos == 0)
                                iCurPos = (signed int)Items.size() - 1;
                            else
                                iCurPos--;
                            bRedraw = true;
                            break;
                        case SDLK_DOWN:
                            if (iCurPos == Items.size() - 1)
                                iCurPos = 0;
                            else
                                iCurPos++;
                            bRedraw = true;
                            break;
                    };
                    break;
                case SDL_QUIT:
                    iCurPos = -1;
                    bEnd = true;
                    break;
            }
        }
        SDL_Delay(50);
        counter++;
        if (counter % 10 == 0)
            bRedraw = true;
    }
    Items.pop_back();
    return iCurPos;
}

void CustomMenu::SetLabels(string menu, string exit) {
    LabelMenu = menu;
    LabelExit = exit;
}

void CustomMenu::SetBitmap(string filename) { FilenameBackground = filename; }
void CustomMenu::SetArea(int x, int y, int w, int h) {
    Rect.x = x;
    Rect.y = y;
    Rect.w = w;
    Rect.h = h;
}
void CustomMenu::SetColors(Uint32 back, Uint32 high) {
    ColorBack = back;
    ColorHighlight = high;
}

SDL_Rect *CustomMenu::GetArea() { return &Rect; }

void CustomMenu::LoadBackground() {
    if (Bitmap != NULL)
        SDL_FreeSurface(Bitmap);

    Bitmap = IMG_Load(FilenameBackground.c_str());
}

void CustomMenu::DrawBitmap(unsigned char alignment, Uint32 color,
                            SDL_Surface *screen) {
    SDL_Rect s, t;

    // SDL_Surface *screen = SDL_GetVideoSurface();

    if ((Bitmap == NULL) || (screen == NULL))
        return;

    s.w = Bitmap->w;
    t.w = Bitmap->w;
    s.h = Bitmap->h;
    t.h = Bitmap->h;
    s.x = 0;
    s.y = 0;
    switch (alignment) {
        case 0:  // LEFT
            t.x = 0;
            t.y = 0;
            break;
        case 1:  // CENTER
            t.x = (screen->w - Bitmap->w) / 2;
            t.y = (screen->h - Bitmap->h) / 2;
            break;
        case 2:  // RIGHT
            t.x = (screen->w - Bitmap->w);
            t.y = (screen->h - Bitmap->h);
            break;
        case 3:  // HORIZONTAL CENTER ONLY
            t.x = (screen->w - Bitmap->w) / 2;
            t.y = 0;
            break;
        case 4:  // VERTICAL CENTER ONLY
            t.x = 0;
            t.y = (screen->h - Bitmap->h) / 2;
            break;
    }
    //   if (Bitmap->format->palette && screen->format->palette)
    //     SDL_SetColors(screen, Bitmap->format->palette->colors, 0, // TODO
    //     SDL 2.0
    //                   Bitmap->format->palette->ncolors);

    SDL_FillRect(screen, &screen->clip_rect, color);

    while (SDL_BlitSurface(Bitmap, &s, screen, &t) == -2) SDL_Delay(50);
}

////////////////////////////////////////
//       SetAlpha
/*! Set alpha blending level
// \param unsigned char alpha :
*/
void CustomMenu::SetAlpha(unsigned char alpha) {
    // SDL_SetAlpha(Screen, SDL_SRCALPHA, alpha);
    SDL_SetSurfaceAlphaMod(Screen, alpha);  // SDL 2.0
}
