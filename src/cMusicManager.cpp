////////////////////////////////////////////////////////////////////////////////
// File         : cMusicManager.cpp
// Project      : 
// Subsystem    : 
// Component    : 
// Author       : Invido.it (c) 2004-2022
// Description  : implementation of the cMusicManager class.
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
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


#include "StdAfx.h"
#include "EngineApp.h" 
#include <SDL.h>
#include "cMusicManager.h"
#include "gfx_util.h"

extern cEngineApp TheApp;

static const char * lpszMusicFile = DATA_PREFIX"watermusic.it";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cMusicManager::cMusicManager()
{
    m_pMusics = 0;
}

cMusicManager::~cMusicManager()
{

}


////////////////////////////////////////
//       Init
/*! Initialize audio stuff
*/
void cMusicManager::Init()
{
    // sound
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr,
  	      "\nWarning: I could not initialize audio!\n"
	      "The Simple DirectMedia error that occured was:\n"
	      "%s\n\n", SDL_GetError());
	       
    }
    if (Mix_OpenAudio(44100, AUDIO_S16, 2, 1024) < 0)
    {
        fprintf(stderr,
	      "\nWarning: I could not set up audio for 44100 Hz "
	      "16-bit stereo.\n"
	      "The Simple DirectMedia error that occured was:\n"
	      "%s\n\n", SDL_GetError());
        
    }

    m_pMusics = Mix_LoadMUS(lpszMusicFile);
    if (TheApp.GetSettings()->bMusicEnabled)
    {
        Mix_PlayMusic(m_pMusics, 0);
    }
}

////////////////////////////////////////
//       StopMusic
/*! Stop play music
*/
void cMusicManager::StopMusic()
{
    Mix_FadeOutMusic(100);
    Mix_HaltMusic();
}



////////////////////////////////////////
//       StartMusic
/*! Start play music
*/
void cMusicManager::StartMusic()
{
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(m_pMusics, 0);
    }
}
