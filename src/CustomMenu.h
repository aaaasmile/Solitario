// #ifndef SDLCUSTOMMENU
// #define SDLCUSTOMMENU

// #ifdef _WINDOWS
// #pragma warning(disable : 4786)
// #endif

// #include <SDL2/SDL.h>
// #include <SDL_image.h>

// #include "ErrorInfo.h"
// #include "Fonts.h"
// #include "win_type_global.h"

// class CustomMenu {
// public:
//     CustomMenu();
//     ~CustomMenu();
//     LPErrInApp Initialize(SDL_Surface *s, SDL_Renderer *r,
//                           std::string filenameBackground);
//     void AddItems(std::string item);
//     void ClearItems();
//     SDL_Rect *GetArea();
//     LPErrInApp LoadBackground();
//     LPErrInApp Run(int &slectedItem);
//     void SetLabels(std::string menu, std::string exit);
//     void SetArea(int x, int y, int w, int h);
//     void SetColors(Uint32 back, Uint32 high);
//     void SetAlpha(unsigned char alpha);

// private:
//     void updateScreenTexture();
//     LPErrInApp drawBitmap(unsigned char alignment, Uint32 color);

// private:
//     std::string _filenameBackground;
//     SDL_Surface *_p_BackgroundBitmap;
//     std::string _labelMenu;
//     std::string _labelExit;

//     CustomFont *_p_Font;

//     signed int _iCurPos;

//     VCT_STRINGS _vctItems;

//     SDL_Rect _rect;
//     Uint32 _colorBack;
//     Uint32 _colorHighlight;
//     SDL_Surface *_p_Screen;
//     SDL_Texture *_p_ScreenTexture;
//     SDL_Renderer *_p_sdlRenderer;
// };

// #endif
