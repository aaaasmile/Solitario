//GameSettings.h
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


#ifndef __GAMESETTINGS__H_
#define __GAMESETTINGS__H_

#include <string>
#include "Languages.h"


//! class cTipoDiMazzo 
/* Collect all info about deck
*/
class cTipoDiMazzo
{
public:
enum eTypeMazzo
{
    PIACENTINA=0,
    BERGAMO,
    BOLOGNA,
    GENOVA,
    MILANO,
    NAPOLI,
    PIEMONTE,
    ROMAGNA,
    SARDEGNA,
    SICILIA,
    TOSCANA,
    TRENTO,
    TREVISO,
    TRIESTE,
};
public:
    cTipoDiMazzo()
    {
    }
public:
    void          SetType(eTypeMazzo eType);
    std::string   GetResFileName(){return m_strResFileName;}
    std::string   GetMazzoName(){return m_strMazzoName;}
    std::string   GetSymbolFileName(){return m_strSymbolName;}
    eTypeMazzo    GetType(){return m_eType;}
    void          Clone(cTipoDiMazzo &Val);
    void          SetTypeIndex(int iVal);
    int           GetTypeIndex(){return (int)m_eType;}
private:
    //! id mazzo
	eTypeMazzo   m_eType;
    //! name
    std::string  m_strMazzoName;
    //! resource file name
    std::string  m_strResFileName;
    //! symbol file name
    std::string  m_strSymbolName;
};

//! class game settings
/**
*/
class cGameSettings
{
public:
   //!player name
   std::string strPlayerName;
   //! delayed animation timer
   long    lDelayTimer;
   //! hand terminate self
   bool    bAutoTerminate;
   //! level of the cpu
   long    lLevel;
   //! player guest identification
   long    lGuestId;
   //! ip remote channel (4 long values)
   long    lIpRemote_1;
   long    lIpRemote_2;
   long    lIpRemote_3;
   long    lIpRemote_4;
   //! deck type
   cTipoDiMazzo         DeckType;
   //! language
   cLanguages::eLangId  eLanguageCurrent;
   bool                 bMusicEnabled;
   
public:
   cGameSettings()
   {
        strPlayerName = "Anonimo";
        lDelayTimer = 3;
        bAutoTerminate = true;
        lLevel = 1;
        lGuestId = 0;
		lIpRemote_1 = 127;
        lIpRemote_2 = 0;
        lIpRemote_3 = 0;
        lIpRemote_4 = 1;
        DeckType.SetType(cTipoDiMazzo::PIACENTINA);
        eLanguageCurrent = cLanguages::LANG_ITA;
        bMusicEnabled = false;
   }
};

#endif
