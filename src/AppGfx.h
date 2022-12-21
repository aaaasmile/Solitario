#ifndef __ENGINE_APP_H
#define __ENGINE_APP_H

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL2/SDL.h>

#include <string>

#include "CGame.h"
#include "CustomMenu.h"
#include "HightScoreMgr.h"
#include "Languages.h"
#include "win_type_global.h"

class MusicManager;

class AppGfx {
public:
    AppGfx();
    ~AppGfx();

    void Init();
    void MainMenu();
    std::string GetPlayerName() { return _p_GameSettings->strPlayerName; }
    void SetPlayerName(std::string strVal) {
        _p_GameSettings->strPlayerName = strVal;
    }
    LanguageMgr* GetLanguageMan() { return &_LanguageMgr; }
    void ParseCmdLine(int argc, char* argv[]);

private:
    int waitKeyLoop();
    int startGameLoop();
    void setVideoResolution();
    void terminate();
    void hightScoreMenu();
    void newGame();
    void handleGameLoopKeyDownEvent(SDL_Event& event);
    void handleGameLoopMouseDownEvent(SDL_Event& event);
    void handleGameLoopMouseMoveEvent(SDL_Event& event);
    void handleGameLoopMouseUpEvent(SDL_Event& event);
    void menuSelectDeck();
    void loadProfile();
    void writeProfile();
    void usage(int err, char* cmd);
    BOOL parseScreenSize(LPCSTR strOpz);
    void updateScreenTexture();

private:
    SDL_Surface* _p_Screen;
    SDL_Surface* _p_imgBackground;
    SDL_Texture* _p_ScreenTexture;
    SDL_Surface* _p_Title;
    SDL_Window* _p_Window;
    SDL_Renderer* _p_sdlRenderer;

    CustomFont* _p_CustomFont;
    GameSettings* _p_GameSettings;
    CGame* _p_SolitarioGfx;
    MusicManager* _p_MusicManager;
    HightScoreMgr _HScoreMgr;
    LanguageMgr _LanguageMgr;

    int _iScreenW;
    int _iScreenH;
    int _iBpp;

    BOOL _bFullScreen;
    BOOL _bStartdrag;
    BOOL _bOverride;
};

#endif
