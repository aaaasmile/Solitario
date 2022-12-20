// EngineApp.h
#ifndef __ENGINE_APP_H
#define __ENGINE_APP_H

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL.h>

#include <string>

#include "CGame.h"
#include "CustomMenu.h"
#include "Languages.h"
#include "cHightScoreMgr.h"
#include "win_type_global.h"

class MusicManager;

class AppGfx {
public:
    AppGfx();
    ~AppGfx();

    void Init();
    void MainMenu();
    std::string GetPlayerName() { return m_Settings.strPlayerName; }
    void SetPlayerName(std::string strVal) {
        m_Settings.strPlayerName = strVal;
    }
    cLanguages* GetLanguageMan() { return &m_LanString; }
    cGameSettings* GetSettings() { return &m_Settings; }
    void Setup(int argc, char* argv[]);

private:
    int WaitKey();
    void setVideoResolution();
    void terminate();
    void hightScoreMenu();
    void NewGame();
    void HandleKeyDownEvent(SDL_Event& event);
    void HandleMouseDownEvent(SDL_Event& event);
    void HandleMouseMoveEvent(SDL_Event& event);
    void HandleMouseUpEvent(SDL_Event& event);
    int PlayGame();
    void menuMazziChoose();
    void loadProfile();
    void writeProfile();
    void usage(int err, char* cmd);
    BOOL parseScreenSize(LPCSTR strOpz);
    void updateScreenTexture();

private:
    SDL_Surface* m_pScreen;
    SDL_Texture* m_pScreenTexture;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_psdlRenderer;
    int m_iScreenW;
    int m_iScreenH;
    int m_iBpp;
    SDL_Surface* m_pimgBackground;
    CustomFont* m_pMainFont;
    HightScoreMgr m_HScore;
    cLanguages m_LanString;
    cGameSettings m_Settings;
    CGame m_SolitarioGfx;
    BOOL m_bStartdrag;
    MusicManager* m_pMusicManager;
    SDL_Surface* m_pTitle;
    BOOL m_bFullScreen;
    BOOL m_bOverride;
};

#endif
