//EngineApp.h
/*
*
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License
*	as published by the Free Software Foundation; either version 2
*	of the License, or (at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*
**/


#ifndef __ENGINE_APP_H
#define __ENGINE_APP_H

#ifdef _WINDOWS
 #pragma warning( disable : 4786  ) 
#endif


#include <SDL.h>
#include "CustomMenu.h"
#include "Languages.h"
#include <string>
#include "CGame.h"
#include "cHightScoreMgr.h"
#include "win_type_global.h"

class cMusicManager;

//! class cEngineApp
class cEngineApp
{
public:
    cEngineApp();
    ~cEngineApp();

    //! init stuff
    void    Init();
    //! application entry point
    void    MainMenu();
    //!options get methods
    std::string     GetPlayerName(){return m_Settings.strPlayerName;}
    void            SetPlayerName(std::string strVal){m_Settings.strPlayerName = strVal;}
    //! language translation
    cLanguages*     GetLanguageMan(){return &m_LanString;}
    cGameSettings*  GetSettings(){return &m_Settings;}
	//! parse command line
	void            Setup(int argc, char * argv[]);
   
private:
    void    mainMenu();
    int     WaitKey();
    void    setVideoResolution();
    void    terminate();
    void    hightScoreMenu();
	void    NewGame();
	void    HandleKeyDownEvent(SDL_Event &event);
	void	HandleMouseDownEvent(SDL_Event &event);
	void    HandleMouseMoveEvent(SDL_Event &event);
	void    HandleMouseUpEvent(SDL_Event &event);
	int		PlayGame();
    void    menuMazziChoose();
    void    loadProfile();
    void    writeProfile();
	void    usage(int err, char * cmd);
	BOOL    parseScreenSize(LPCSTR strOpz);

private:
    //! main screen surface
    SDL_Surface*      m_pScreen;
    //! screen width
    int               m_iScreenW;
    //! screen height
    int               m_iScreenH;
    //! bit per pixel
    int               m_iBpp;
    //! image background
    SDL_Surface*      m_pimgBackground;
    //! font used
    sdlFont*          m_pMainFont;
    //! hight score data
    cHightScoreMgr    m_HScore;
    //! get strings on differnt languages
    cLanguages        m_LanString;
    //! game settings
    cGameSettings     m_Settings;
	//! grafic game engines
	CGame             m_SolitarioGfx;
	//! drag flag
    BOOL              m_bStartdrag;
    //! music manager
    cMusicManager*    m_pMusicManager;
	//! title
    SDL_Surface*      m_pTitle;
	//! full screen flag
	BOOL              m_bFullScreen;
	//! override settings
	BOOL              m_bOverride;

};


#endif
