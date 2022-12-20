////////////////////////////////////////////////////////////////////////////////
// File         : Languages.cpp
// Project      : 
// Subsystem    : 
// Component    : 
// Author       : Igor Sarzi Sartori (c) 2004
// Description  : 
////////////////////////////////////////////////////////////////////////////////
// History      : 8.05.2004   Igor Sarzi Sartori    Created

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


#include "Languages.h"
#include <assert.h>


////////////////////////////////////////
//       cLanguages
/*! constructor
*/
cLanguages::cLanguages()
{
    for (int i = 0; i< TOT_STRINGS; i++)
    {
        for (int j = 0; j < TOT_LANG; j++)
        {
            m_matStringsLang[j][i] = "No TRanslation!";
        }
    }
    m_eLangid = LANG_ITA;
    //dialetto
    m_matStringsLang[LANG_DIAL_MN][ID_START] = "Cümincia";
    m_matStringsLang[LANG_DIAL_MN][ID_LANGUAGESEL] = "Lengui furèsti";
    m_matStringsLang[LANG_DIAL_MN][ID_HISCORE] = "Al mèi di mèi";
    m_matStringsLang[LANG_DIAL_MN][ID_VIDEOSET] = "Giösta al display";
    m_matStringsLang[LANG_DIAL_MN][ID_MAINMENU] = "Menu principal";
    m_matStringsLang[LANG_DIAL_MN][ID_EXIT] = "Fnomla che";
    m_matStringsLang[LANG_DIAL_MN][ID_CHOOSELEVEL] = "Cata föra al jeuq";
    m_matStringsLang[LANG_DIAL_MN][ID_BACKMAINMENU] = "Al menu principal";
    m_matStringsLang[LANG_DIAL_MN][ID_EASY] = "Facil";
    m_matStringsLang[LANG_DIAL_MN][ID_HARD] = "Dificilisim";
    m_matStringsLang[LANG_DIAL_MN][ID_HARDER] = "Mia tant facil";
    m_matStringsLang[LANG_DIAL_MN][ID_BACK] = "Turna indrè";
    m_matStringsLang[LANG_DIAL_MN][ID_SELECTDISPLAYSETTINGS] = "Cata föra al display";
    m_matStringsLang[LANG_DIAL_MN][ID_INVIDO] = "Invidu";
    m_matStringsLang[LANG_DIAL_MN][ID_CHOOSELANGUA] = "Cata föra la lengua";
    m_matStringsLang[LANG_DIAL_MN][ID_ENGLISH] = "L'ingles";
    m_matStringsLang[LANG_DIAL_MN][ID_DIALETMN] = "Al dialet";
    m_matStringsLang[LANG_DIAL_MN][ID_ITALIANO] = "L'italian";
    m_matStringsLang[LANG_DIAL_MN][ID_ANONIM] = "Anònim";
    m_matStringsLang[LANG_DIAL_MN][ID_PUSHBUTTON] = "Schesa an buton";
    m_matStringsLang[LANG_DIAL_MN][ID_CMD_UNKNOWN] = "Cumand mia cunusi";
    m_matStringsLang[LANG_DIAL_MN][ID_CMD_HELP] = "Cumand cat po catar fora:";
    m_matStringsLang[LANG_DIAL_MN][ID_CMD_CHATWELCOME] = "Benarriva' in dla chat";
    m_matStringsLang[LANG_DIAL_MN][ID_CMD_CHATLIST] = "La lista adla gent in dla chat:";
    m_matStringsLang[LANG_DIAL_MN][ID_SOLITARIO] = "Sulitari";
    m_matStringsLang[LANG_DIAL_MN][ID_MAZZONAME] = "Mas ad li carti";
    m_matStringsLang[LANG_DIAL_MN][ID_SOUNDOPT] = "Müsica";
    m_matStringsLang[LANG_DIAL_MN][ID_ABILITATO] = "Impiàda";
    m_matStringsLang[LANG_DIAL_MN][ID_DISABILITATO] = "Smorsa";

    m_matStringsLang[LANG_DIAL_MN][ID_CHOOSEMAZZO] = "Cata föra al mas ad lì carti";
    m_matStringsLang[LANG_DIAL_MN][ID_PIACENTINA] = "Piacentini";
    m_matStringsLang[LANG_DIAL_MN][ID_BERGAMO] = "Bergum";
    m_matStringsLang[LANG_DIAL_MN][ID_BOLOGNA] = "Bulugnesi";
    m_matStringsLang[LANG_DIAL_MN][ID_GENOVA] = "Genuesi";
    m_matStringsLang[LANG_DIAL_MN][ID_MILANO] = "Milanesi";
    m_matStringsLang[LANG_DIAL_MN][ID_NAPOLI] = "Napuletani";
    m_matStringsLang[LANG_DIAL_MN][ID_PIEMONTE] = "Piemuntesi";
    m_matStringsLang[LANG_DIAL_MN][ID_ROMAGNA] = "Rumagnoli";
    m_matStringsLang[LANG_DIAL_MN][ID_SARDEGNA] = "Sardagnöli";
    m_matStringsLang[LANG_DIAL_MN][ID_TOSCANA] = "Tuscani";
    m_matStringsLang[LANG_DIAL_MN][ID_TRENTO] = "Trentini";
    m_matStringsLang[LANG_DIAL_MN][ID_TREVISO] = "Trevisani";
    m_matStringsLang[LANG_DIAL_MN][ID_TRIESTE] = "triestini";
    m_matStringsLang[LANG_DIAL_MN][ID_SICILIA] = "Siciliani";
    m_matStringsLang[LANG_DIAL_MN][ID_CREDITS] = "Ringrasiament";

    // italiano
    m_matStringsLang[LANG_ITA][ID_START] = "Inizia";
    m_matStringsLang[LANG_ITA][ID_LANGUAGESEL] = "Lingua";
    m_matStringsLang[LANG_ITA][ID_HISCORE] = "Hight Score";
    m_matStringsLang[LANG_ITA][ID_VIDEOSET] = "Settings Display";
    m_matStringsLang[LANG_ITA][ID_MAINMENU] = "Menu principale";
    m_matStringsLang[LANG_ITA][ID_EXIT] = "Esci";
    m_matStringsLang[LANG_ITA][ID_CHOOSELEVEL] = "Seleziona il gioco";
    m_matStringsLang[LANG_ITA][ID_BACKMAINMENU] = "Vai al menu principale";
    m_matStringsLang[LANG_ITA][ID_EASY] = "Facile";
    m_matStringsLang[LANG_ITA][ID_HARD] = "Difficilissimo";
    m_matStringsLang[LANG_ITA][ID_HARDER] = "Non facile";
    m_matStringsLang[LANG_ITA][ID_BACK] = "Torna indietro";
    m_matStringsLang[LANG_ITA][ID_SELECTDISPLAYSETTINGS] = "Seleziona il display";
    m_matStringsLang[LANG_ITA][ID_INVIDO] = "Invidu";
    m_matStringsLang[LANG_ITA][ID_CHOOSELANGUA] = "Seleziona la lingua";
    m_matStringsLang[LANG_ITA][ID_ENGLISH] = "Inglese";
    m_matStringsLang[LANG_ITA][ID_DIALETMN] = "Dialetto Bredese";
    m_matStringsLang[LANG_ITA][ID_ITALIANO] = "Italiano";
    m_matStringsLang[LANG_ITA][ID_ANONIM] = "Anonimo";
    m_matStringsLang[LANG_ITA][ID_PUSHBUTTON] = "Premi un tasto";
    m_matStringsLang[LANG_ITA][ID_CMD_UNKNOWN] = "Comando non riconosciuto";
    m_matStringsLang[LANG_ITA][ID_CMD_HELP] = "Comandi che puoi usare";
    m_matStringsLang[LANG_ITA][ID_CMD_CHATWELCOME] = "Bene arrivato nella chat";
    m_matStringsLang[LANG_ITA][ID_CMD_CHATLIST] = "La lista dei presenti nella chat";
    m_matStringsLang[LANG_ITA][ID_SOUNDOPT] = "Suono";
    m_matStringsLang[LANG_ITA][ID_ABILITATO] = "Abilitato";
    m_matStringsLang[LANG_ITA][ID_DISABILITATO] = "Disabilitato";

    m_matStringsLang[LANG_ITA][ID_SOLITARIO] = "Solitario";
    m_matStringsLang[LANG_ITA][ID_MAZZONAME] = "Mazzi di carte";
    m_matStringsLang[LANG_ITA][ID_CHOOSEMAZZO] = "Scegli il mazzo di carte";
    m_matStringsLang[LANG_ITA][ID_PIACENTINA] = "Piacentine";
    m_matStringsLang[LANG_ITA][ID_BERGAMO] = "Bergamesche";
    m_matStringsLang[LANG_ITA][ID_BOLOGNA] = "Bolognesi";
    m_matStringsLang[LANG_ITA][ID_GENOVA] = "Genovesi";
    m_matStringsLang[LANG_ITA][ID_MILANO] = "Milanesi";
    m_matStringsLang[LANG_ITA][ID_NAPOLI] = "Napoletane";
    m_matStringsLang[LANG_ITA][ID_PIEMONTE] = "Piemontesi";
    m_matStringsLang[LANG_ITA][ID_ROMAGNA] = "Romagnole";
    m_matStringsLang[LANG_ITA][ID_SARDEGNA] = "Sarde";
    m_matStringsLang[LANG_ITA][ID_TOSCANA] = "Toscane";
    m_matStringsLang[LANG_ITA][ID_TRENTO] = "Trentine";
    m_matStringsLang[LANG_ITA][ID_TREVISO] = "Trevigiane";
    m_matStringsLang[LANG_ITA][ID_TRIESTE] = "Triestine";
    m_matStringsLang[LANG_ITA][ID_SICILIA] = "Siciliane";
    m_matStringsLang[LANG_ITA][ID_CREDITS] = "Credits";

    /*
    // inglese
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
    m_matStringsLang[][] = "";
*/

}



////////////////////////////////////////
//       GetStringId
/*! 
// \param eStringID eId : 
*/
std::string cLanguages::GetStringId(eStringID eId)
{
    std::string strRet ="Err trans.";

    assert(eId < TOT_STRINGS);

    if (eId < TOT_STRINGS)
    {
        strRet = m_matStringsLang[m_eLangid][eId];
    }

    return strRet;
}


////////////////////////////////////////
//       GetCStringId
/*! 
// \param eStringID eId : 
*/
char*  cLanguages::GetCStringId(eStringID eId)
{
    std::string strRet ="Err trans.";

    assert(eId < TOT_STRINGS);

    if (eId < TOT_STRINGS)
    {
        strRet = m_matStringsLang[m_eLangid][eId];
    }

    return const_cast<char*>(strRet.c_str()) ;
}
