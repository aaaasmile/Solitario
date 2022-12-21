#ifndef SDLCUSTOMMENU
#define SDLCUSTOMMENU

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "error_info.h"
#include "fonts.h"
#include "win_type_global.h"

using namespace std;

class CustomMenu {
public:
    string FilenameBackground;

    CustomMenu();
    ~CustomMenu();
    LPErrInApp Initialize();
    void AddItems(string item);
    void ClearItems();
    void DrawBitmap(unsigned char alignment, Uint32 color, SDL_Surface *screen);
    SDL_Rect *GetArea();
    void LoadBackground();
    int Run();
    void SetLabels(string menu, string exit);
    void SetBitmap(string filename);
    void SetArea(int x, int y, int w, int h);
    void SetColors(Uint32 back, Uint32 high);
    void SetScreen(SDL_Surface *pVal) { Screen = pVal; }
    void SetAlpha(unsigned char alpha);

private:
    SDL_Surface *Bitmap;
    string LabelMenu;
    string LabelExit;

    CustomFont *Font;

    signed int iCurPos;

    VCT_STRINGS Items;

    SDL_Rect Rect;
    Uint32 ColorBack;
    Uint32 ColorHighlight;
    SDL_Surface *Screen;
};

#endif
