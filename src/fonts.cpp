#include "fonts.h"

#include "win_type_global.h"

sdlFont::sdlFont() { Bitmap = NULL; }
sdlFont::~sdlFont() { ClearFont(); }
void sdlFont::ClearFont() {
    if (Bitmap != NULL) {
        SDL_FreeSurface(Bitmap);
        Bitmap = NULL;
    }
}

////////////////////////////////////////
//       LoadFont
/*!
// \param string filename :
*/
bool sdlFont::LoadFont(string filename) {
    ClearFont();
    Bitmap = SDL_LoadBMP(filename.c_str());
    if (Bitmap == 0) {
        fprintf(stderr, "Font %s not  found\n", filename.c_str());
        exit(1);
    }
    SDL_SetColorKey(Bitmap, TRUE, SDL_MapRGB(Bitmap->format, 0, 0, 0));

    if (Bitmap != NULL) {
        Filename = filename;
        return true;
    }
    return false;
}
void sdlFont::DrawString(SDL_Surface *surface, string message,
                         unsigned char textcase, char alignment, int x, int y,
                         unsigned int color) {
    unsigned char c;
    int ax;
    rSource.w = SDLFONTSIZE;
    rSource.h = SDLFONTSIZE;
    rTarget.w = SDLFONTSIZE;
    rTarget.h = SDLFONTSIZE;

    switch (alignment) {
        case TEXTALIGNLEFT:
            ax = 0;
            break;
        case TEXTALIGNRIGHT:
            ax = surface->w - ((int)message.size() * SDLFONTSIZE);
            break;
        case TEXTALIGNCENTER:
            ax = (surface->w / 2) - (((int)message.size() * SDLFONTSIZE) / 2);
            break;
        default:
            ax = 0;
            break;
    };

    for (unsigned int k = 0; k < message.size(); k++) {
        rTarget.y = y;
        rTarget.x = x + k * SDLFONTSIZE + ax;
        c = message[k];
        switch (textcase) {
            case TEXTUPPER:
                if ((c >= 'a') && (c <= 'z'))
                    c -= 32;
                break;
            case TEXTLOWER:
                if ((c >= 'A') && (c <= 'Z'))
                    c += 32;
                break;
            default:
                break;
        };
        if (c < 32)
            c = 0;
        if (c != 0) {
            rSource.x = SDLFONTSIZE * (c % 16);
            rSource.y = SDLFONTSIZE * (c / 16);
            SDL_BlitSurface(Bitmap, &rSource, surface, &rTarget);
        }
    }
}
