
#ifndef _MSG_BOX_H__
#define _MSG_BOX_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include <vector>

#include "ErrorInfo.h"
#include "Traits.h"
#include "WinTypeGlobal.h"

using namespace traits;

class ButtonGfx;

class MesgBoxGfx {
public:
    MesgBoxGfx(void);
    ~MesgBoxGfx(void);

    enum eMSGBOX_TYPE { MBOK, MB_YES_NO };
    enum {
        MB_RES_NO = 0,
        MB_RES_YES = 1,
        MB_ID_BUTT_YES = 2,
        MB_ID_BUTT_NO = 3,
        MB_ID_OK = 4
    };

    LPErrInApp Initialize(SDL_Rect* pRect, SDL_Surface* pScreen,
                          TTF_Font* pFont, eMSGBOX_TYPE eval,
                          SDL_Renderer* pRenderer);
    int Show(SDL_Surface* pScene_background, LPCSTR lpsBut1_txt,
             LPCSTR lpsBut2_txt, LPCSTR lpsMsg_txt);
    void ButCmdClicked(int iButID);
    void AddLineText(LPCSTR strLine) { _dataStrings.push_back(strLine); }
    void ChangeTextColor(SDL_Color newColor) { _colCurrent = newColor; }
    void ChangeAlpha(Uint8 newAlpha) { _alpha = newAlpha; }

private:
    ClickCb prepClickBtCb();

private:
    SDL_Renderer* _p_sdlRenderer;
    SDL_Rect _rctMsgBox;
    STRING _strMsgText;
    SDL_Surface* _p_Surf_Bar;
    SDL_Surface* _p_Screen;
    TTF_Font* _p_FontText;
    SDL_Color _colCurrent;
    eMSGBOX_TYPE _typeMsg;
    ButtonGfx* _p_BtButt1;
    ButtonGfx* _p_BtButt2;
    bool _terminated;
    int _result;
    VCT_STRING _dataStrings;
    Uint8 _alpha;
};

#endif
