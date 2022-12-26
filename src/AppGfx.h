#ifndef APPGFX__H
#define APPGFX__H

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "CustomMenu.h"
#include "ErrorInfo.h"
#include "Fonts.h"
#include "GameGfx/SolitarioGfx.h"
#include "HightScoreMgr.h"
#include "Languages.h"
#include "Traits.h"
#include "win_type_global.h"

class MusicManager;

using namespace traits;

class AppGfx {
public:
    AppGfx();
    ~AppGfx();

    LPErrInApp Init();
    LPErrInApp StartMainMenu();  // TODO becoming obsolete
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
    void SetNextMenu(int iVal);

private:
    int waitKeyLoop();
    LPErrInApp startGameLoop();
    LPErrInApp createWindow();
    void terminate();
    void hightScoreMenu();
    void newGame();
    void handleGameLoopKeyDownEvent(SDL_Event& event);
    void handleGameLoopMouseDownEvent(SDL_Event& event);
    void handleGameLoopMouseMoveEvent(SDL_Event& event);
    void handleGameLoopMouseUpEvent(SDL_Event& event);
    LPErrInApp menuSelectDeck();
    LPErrInApp loadProfile();
    void writeProfile();
    void usage(int err, char* cmd);
    BOOL parseScreenSize(LPCSTR strOpz);
    void updateScreenTexture();
    LPErrInApp showHelp();
    LPErrInApp showCredits();
    LPErrInApp showOptionGeneral();

    MenuDelegator prepMenuDelegator();

private:
    SDL_Surface* _p_Screen;
    SDL_Surface* _p_imgBackground;
    SDL_Texture* _p_ScreenTexture;
    SDL_Surface* _p_Title;
    SDL_Window* _p_Window;
    SDL_Renderer* _p_sdlRenderer;

    CustomFont* _p_CustomFont;
    TTF_Font* _p_fontVera;
    TTF_Font* _p_fontAriblk;
    GameSettings* _p_GameSettings;
    SolitarioGfx* _p_SolitarioGfx;
    MusicManager* _p_MusicManager;
    HightScoreMgr _HScoreMgr;
    Languages _LanguageMgr;

    int _iScreenW;
    int _iScreenH;
    int _iBpp;

    BOOL _bFullScreen;
    BOOL _bStartdrag;
    BOOL _bOverride;
};

#endif
