#ifndef APPGFX__H
#define APPGFX__H

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <stack>
#include <string>

#include "ErrorInfo.h"
#include "GameGfx/SolitarioGfx.h"
#include "Languages.h"
#include "Traits.h"
#include "win_type_global.h"

class MusicManager;
class MenuMgr;

using namespace traits;

class AppGfx {
public:
    AppGfx();
    ~AppGfx();

    LPErrInApp Init();
    LPErrInApp MainLoop();
    std::string GetPlayerName() { return _p_GameSettings->strPlayerName; }
    void SetPlayerName(std::string strVal) {
        _p_GameSettings->strPlayerName = strVal;
    }
    Languages* GetLanguageMan() { return &_LanguageMgr; }
    void ParseCmdLine(int argc, char* argv[]);
    TTF_Font* GetFontVera() { return _p_fontVera; }
    TTF_Font* GetFontAriblk() { return _p_fontAriblk; }
    void LeaveMenu();
    void SetNextMenu(int iVal) { _Histmenu.push(iVal); }

private:
    int waitKeyLoop();
    LPErrInApp startGameLoop();
    LPErrInApp createWindow();
    void terminate();
    void hightScoreMenu();
    LPErrInApp newGame();
    LPErrInApp handleGameLoopKeyDownEvent(SDL_Event& event);
    LPErrInApp handleGameLoopMouseDownEvent(SDL_Event& event);
    void handleGameLoopMouseMoveEvent(SDL_Event& event);
    LPErrInApp handleGameLoopMouseUpEvent(SDL_Event& event);
    LPErrInApp loadProfile();
    void writeProfile();
    void usage(int errOut, char* cmd);
    BOOL parseScreenSize(LPCSTR strOpz);
    void updateScreenTexture();
    LPErrInApp showHelp();
    LPErrInApp showCredits();
    LPErrInApp showOptionGeneral();

    MenuDelegator prepMenuDelegator();
    void drawSplash();
    LPErrInApp loadSplash();

private:
    SDL_Surface* _p_Screen;
    SDL_Surface* _p_Splash;
    SDL_Surface* _p_Title;
    SDL_Texture* _p_ScreenTexture;
    SDL_Window* _p_Window;
    SDL_Renderer* _p_sdlRenderer;
    TTF_Font* _p_fontVera;
    TTF_Font* _p_fontAriblk;
    GameSettings* _p_GameSettings;
    SolitarioGfx* _p_SolitarioGfx;
    MusicManager* _p_MusicManager;
    Languages _LanguageMgr;

    int _iScreenW;
    int _iScreenH;
    int _iBpp;

    BOOL _bFullScreen;
    BOOL _bStartdrag;
    BOOL _bOverride;

    std::stack<int> _Histmenu;
};

#endif
