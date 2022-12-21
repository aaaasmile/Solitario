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
    LPErrInApp DrawBitmap(unsigned char alignment, Uint32 color,
                          SDL_Surface *screen);
    SDL_Rect *GetArea();
    LPErrInApp LoadBackground();
    LPErrInApp Run(int &slectedItem);
    void SetLabels(string menu, string exit);
    void SetBitmap(string filename);
    void SetArea(int x, int y, int w, int h);
    void SetColors(Uint32 back, Uint32 high);
    void SetScreen(SDL_Surface *pVal) { _p_Screen = pVal; }
    void SetAlpha(unsigned char alpha);

private:
    SDL_Surface *_p_BackgroundBitmap;
    string _labelMenu;
    string _labelExit;

    CustomFont *_p_Font;

    signed int _iCurPos;

    VCT_STRINGS _vctItems;

    SDL_Rect _rect;
    Uint32 _colorBack;
    Uint32 _colorHighlight;
    SDL_Surface *_p_Screen;
};

#endif
