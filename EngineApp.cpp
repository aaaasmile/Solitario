//EngineApp.cpp
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

#include "win_type_global.h"

#include "StdAfx.h"

#include <stdlib.h>
#include <time.h>
#include "gfx_util.h"
#include "EngineApp.h"
#include "cMusicManager.h"
#ifdef _WINDOWS
    #include "regkey.h"
#else
    #include <libini.h>
#endif
#include "credits.h"
#include "fading.h"

#define 	CRD_PILE			0
#define 	CRD_FOUNDATION		1
#define 	CRD_RESERVE 	    2
#define		CRD_WASTE   		3

// globals
SDL_Surface   *screen;

typedef std::vector<std::string> VCT_STRINGS;



//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// class cEngineApp ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS
	static LPCSTR strIDS_KEY_LASTPATH = "SOFTWARE\\BredaSoft\\Solitario";
	static LPCSTR strIDS_KEY_PLAYERNAME = "PlayerName";
	static LPCSTR strIDS_KEY_LANGUAGECURRENT = "Language";
	static LPCSTR strIDS_KEY_DECKCURRENT = "DeckCurrent";
	static LPCSTR strIDS_KEY_MUSICENABLED = "Musicenabled";
#else
	static const char * lpszIniFileName = DATA_PREFIX"options.ini";
	static const char * lpszSectAll = "global";
	static const char * lpszKeyLang = "Language";
	static const char * lpszKeyDeck = "DeckCurrent";
	static const char * lpszKeyMusic = "Musicenabled";
#endif

static const char * lpszIconProgFile = DATA_PREFIX"icona_asso.bmp";
static const char * lpszFontFile = DATA_PREFIX"images/font.bmp";
static const char * lpszTitleFile = DATA_PREFIX"images/title.png";
static const char * lpszBackGroundFile = DATA_PREFIX"im001537.jpg";

////////////////////////////////////////
//       cEngineApp
/*! constructor
*/
cEngineApp::cEngineApp()
{
    m_pScreen = NULL;
    m_iScreenW = 1024 ; //640;
    m_iScreenH = 768 ; //480;
    m_iBpp = 0;
    m_pimgBackground = NULL;
    m_pMainFont = NULL;
    m_bStartdrag = FALSE;
    m_pMusicManager = 0;
	m_pTitle = 0;
	m_bFullScreen = FALSE;
}


////////////////////////////////////////
//       ~cEngineApp
/*! destructor
*/
cEngineApp::~cEngineApp()
{
    terminate();
}


////////////////////////////////////////
//       NewGame
/*! Start a new game 
*/
void cEngineApp::NewGame()
{
    //Reset pile symbol
    m_SolitarioGfx[0].SetSymbol(CRD_OSYMBOL);
    
    //Empty the card regions from the previous game
    m_SolitarioGfx.EmptyStacks();
    
    //create then shuffle the deck
    m_SolitarioGfx[0].NewDeck();
    m_SolitarioGfx[0].Shuffle();
    
    //deal
	int i;
    for(i=1; i <= 7; i++)
    {
        m_SolitarioGfx[i].Push(m_SolitarioGfx[0].Pop(i));
    }
    
    //initialize all card coordinates
    m_SolitarioGfx.InitAllCoords();
    
    //set initial faced up cards in foundations
    for(i=1; i <= 7; i++)
    {
        m_SolitarioGfx[i].SetCardFaceUp(TRUE, m_SolitarioGfx[i].Size() - 1);
    }
}


////////////////////////////////////////
//       HandleKeyDownEvent
/*! User press a key on keyboard
// \param SDL_Event &event : 
*/
void cEngineApp::HandleKeyDownEvent(SDL_Event &event)
{
    if(event.key.keysym.sym == SDLK_n)		
    { 
        NewGame(); 
        m_SolitarioGfx.DrawBackground(FALSE);
        m_SolitarioGfx.DrawStaticScene(); 
    }
    if(event.key.keysym.sym == SDLK_a)		
    { 
        m_SolitarioGfx.AnimateCards(); 
    }; // Test animation
    if(event.key.keysym.sym == SDLK_r)		
    { 
        m_SolitarioGfx.DrawStaticScene(); 
    }; // Refresh
}



////////////////////////////////////////
//       HandleMouseDownEvent
/*! 
// \param SDL_Event &event : 
*/
void cEngineApp::HandleMouseDownEvent(SDL_Event &event)
{
    CCardRegion *srcReg;
    if(event.button.button == SDL_BUTTON_LEFT)
    {
        srcReg = m_SolitarioGfx.OnMouseDown(event.button.x, event.button.y);
        if(srcReg == NULL) 
            return;
        //clicked on the top of the foundations
        if((srcReg->Id == CRD_FOUNDATION) && srcReg->PtOnTop(event.button.x, event.button.y))
        {
            srcReg->SetCardFaceUp(TRUE, srcReg->Size() - 1);
        }
        //clicked on the foundations, reserve, wastes for dragging
        if(((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_RESERVE) || (srcReg->Id == CRD_WASTE)) && m_SolitarioGfx.InitDrag(event.button.x, event.button.y))
        {
            m_bStartdrag = TRUE;
            SDL_WM_GrabInput(SDL_GRAB_ON);
        }
        //clicked on the pile
        if(srcReg->Id == CRD_PILE)
        {
            CCardStack *cs = new CCardStack;
            if(srcReg->Empty() && !m_SolitarioGfx[8].Empty()) //Bring back the cards
            {
                *cs = m_SolitarioGfx[8].Pop(m_SolitarioGfx[8].Size());
                cs->SetCardsFaceUp(FALSE);
                m_SolitarioGfx.InitDrag(cs, -1, -1);
                m_SolitarioGfx.DoDrop(&m_SolitarioGfx[0]);
                m_SolitarioGfx[0].Reverse();
                m_SolitarioGfx[0].InitCardCoords();
            }
            
            else if(!srcReg->Empty() && (!m_SolitarioGfx[8].Empty() || m_SolitarioGfx[8].Empty()))
            {
                *cs = m_SolitarioGfx[0].Pop(1);
                cs->SetCardsFaceUp(TRUE);
                m_SolitarioGfx.InitDrag(cs, -1, -1);
                m_SolitarioGfx.DoDrop(&m_SolitarioGfx[8]);
            }
        }
    }
    
    //substitute right-click for double-click event
    if(event.button.button == SDL_BUTTON_RIGHT)
    {
        srcReg = m_SolitarioGfx.OnMouseDown(event.button.x, event.button.y);
        if(srcReg == NULL) return;
        CCardRegion *cr;
        CCard card =  srcReg->GetCard(srcReg->Size()-1);
        
        //clicked on the top of the foundations
        if(((srcReg->Id == CRD_FOUNDATION) || (srcReg->Id == CRD_RESERVE)) && card.FaceUp() && srcReg->PtOnTop(event.button.x, event.button.y))
        {
			cr = m_SolitarioGfx.FindDropRegion(CRD_WASTE, card);
            if(cr)
            {
                CCardStack *cs = new CCardStack;
                *cs = srcReg->Pop(1);
                m_SolitarioGfx.InitDrag(cs, -1 , -1);
                m_SolitarioGfx.DoDrop(cr);
            }
        }
    }
}


////////////////////////////////////////
//       HandleMouseMoveEvent
/*! 
// \param SDL_Event &event : 
*/
void cEngineApp::HandleMouseMoveEvent(SDL_Event &event)
{
    if(event.motion.state == SDL_BUTTON(1) && m_bStartdrag)
        m_SolitarioGfx.DoDrag(event.motion.x, event.motion.y);
}


////////////////////////////////////////
//       HandleMouseUpEvent
/*! 
// \param SDL_Event &event : 
*/
void cEngineApp::HandleMouseUpEvent(SDL_Event &event)
{
    if(m_bStartdrag)
    {
        m_bStartdrag = FALSE;
        m_SolitarioGfx.DoDrop();
        SDL_WM_GrabInput(SDL_GRAB_OFF);
    }
    if(m_SolitarioGfx[0].Empty() && m_SolitarioGfx[8].Empty())
    {
        m_SolitarioGfx[0].SetSymbol(1);
        m_SolitarioGfx.DrawStaticScene();
    }
    //victory
    if((m_SolitarioGfx[9].Size() == 10) && (m_SolitarioGfx[10].Size() == 10) && (m_SolitarioGfx[11].Size() == 10) && (m_SolitarioGfx[12].Size() == 10))
    {
        m_SolitarioGfx.AnimateCards();
        NewGame();
        m_SolitarioGfx.DrawStaticScene();
    }
}



////////////////////////////////////////
//       terminate
/*! Terminate stuff
*/
void cEngineApp::terminate()
{
    // save hight score
    //m_HScore.Save();
    //save settings
    writeProfile();

    SDL_ShowCursor( SDL_ENABLE );
    
    if (m_pimgBackground!=NULL)
    {
        SDL_FreeSurface(m_pimgBackground);
        m_pimgBackground = NULL;
    }
    delete m_pMainFont;
    if (m_pScreen!=NULL)
    {
        SDL_FreeSurface(m_pScreen);
        m_pScreen=NULL;
    }
    delete m_pMusicManager;
    SDL_Quit();
}



////////////////////////////////////////
//       loadProfile
/*! Read settings in the registry. 
*/
void cEngineApp::loadProfile()
{
#ifdef _WINDOWS	 
    RegistryKey RegKey;
    LONG lRes;

    lRes = RegKey.Open(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH);  

    if (!lRes)
    {
        // player name
        m_Settings.strPlayerName = RegKey.getRegStringValue(m_Settings.strPlayerName.c_str() , strIDS_KEY_PLAYERNAME);
        // deck type
        int iVal = RegKey.getRegDWordValue(m_Settings.DeckType.GetTypeIndex(), strIDS_KEY_DECKCURRENT);
        m_Settings.DeckType.SetTypeIndex(iVal); 

        // language
        iVal = RegKey.getRegDWordValue(m_Settings.eLanguageCurrent, strIDS_KEY_LANGUAGECURRENT); 
        switch(iVal)
        {
            case 0:
                m_Settings.eLanguageCurrent = cLanguages::LANG_ITA;
                break;
            case 1:
                m_Settings.eLanguageCurrent = cLanguages::LANG_DIAL_MN;
                break;
        }
        // music
        iVal = RegKey.getRegDWordValue(0, strIDS_KEY_MUSICENABLED); 
		if(!m_bOverride)
		{
			if(iVal==0 )
			{
				m_Settings.bMusicEnabled = FALSE;
			}
			else
			{
				m_Settings.bMusicEnabled = TRUE;
			}
		}
    }
    else
    {
        //  create a key
        RegKey.Create(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH); 
        
    } 
    RegKey.Close(); 
#else
 	// linux version
 	ini_fd_t  pIni = ini_open(lpszIniFileName,"r", "#");
  	

	if (pIni == NULL)
		return;

	int iVal;
	
    // deck type
    iVal = m_Settings.DeckType.GetTypeIndex();
    if (pIni)
    {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyDeck);
        ini_readInt(pIni, &iVal);
    }
	m_Settings.DeckType.SetTypeIndex(iVal);
	// language
	iVal = m_Settings.eLanguageCurrent;
	if (pIni)
    {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyLang);
        ini_readInt(pIni, &iVal);
    }
	m_Settings.eLanguageCurrent = (cLanguages::eLangId)iVal;
	// music
	iVal = FALSE;
	if (pIni)
    {
        ini_locateHeading(pIni, lpszSectAll);
        ini_locateKey(pIni, lpszKeyMusic);
        ini_readInt(pIni, &iVal);
    }
	if (!m_bOverride)
	{
		m_Settings.bMusicEnabled = iVal;
	}
	
	ini_close(pIni);
#endif
}


////////////////////////////////////////
//       writeProfile
/*! Save current settings in the registry
*/
void cEngineApp::writeProfile()
{
#ifdef _WINDOWS
    RegistryKey RegKey;
    LONG lRes;
    lRes = RegKey.Open(HKEY_CURRENT_USER, strIDS_KEY_LASTPATH);  
    if (!lRes)
    {
        RegKey.setRegStringValue(m_Settings.strPlayerName.c_str() , strIDS_KEY_PLAYERNAME);
        RegKey.setRegDWordValue(m_Settings.eLanguageCurrent, strIDS_KEY_LANGUAGECURRENT); 
        RegKey.setRegDWordValue(m_Settings.DeckType.GetType(), strIDS_KEY_DECKCURRENT);
        RegKey.setRegDWordValue(m_Settings.bMusicEnabled, strIDS_KEY_MUSICENABLED);
        RegKey.Close();
    }
#else

	ini_fd_t  pIni = ini_open(lpszIniFileName,"w", "#");
	if (pIni == 0)
       return;
	
	// deck type
	ini_locateHeading(pIni, lpszSectAll);
	ini_locateKey(pIni, lpszKeyDeck);
	ini_writeInt(pIni, (int)m_Settings.DeckType.GetType() );
	
	// language
	ini_locateHeading(pIni, lpszSectAll);
	ini_locateKey(pIni, lpszKeyLang);
	ini_writeInt(pIni, m_Settings.eLanguageCurrent);
	
	//music
	ini_locateHeading(pIni, lpszSectAll);
    ini_locateKey(pIni, lpszKeyMusic);
    ini_writeInt(pIni, m_Settings.bMusicEnabled );
    
	ini_close(pIni);
#endif
}


////////////////////////////////////////
//       Init
/*! Init application
*/
void cEngineApp::Init()
{
    // load setting in the registry
    loadProfile();

    // Initialize SDL 
    if ( SDL_Init(0) < 0 ) 
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        exit(1);
    }    
    setVideoResolution();

    m_pMusicManager = new cMusicManager;
    m_pMusicManager->Init(); 
    
    // set application language
    m_LanString.SetLang(m_Settings.eLanguageCurrent);
    
    m_pMainFont=new sdlFont;
    //LOAD BITMAP FONT
    m_pMainFont->LoadFont(lpszFontFile);	
    
    //HIDE MOUSE
    //SDL_ShowCursor( SDL_DISABLE );
    
    // caption
    SDL_WM_SetCaption(m_LanString.GetCStringId(cLanguages::ID_SOLITARIO) ,NULL);	
    
    // hight score
    //m_HScore.Load(); 
    
    // icona trasparente
    SDL_Surface * psIcon = SDL_LoadBMP(lpszIconProgFile);
    if (psIcon == 0)
    {
        fprintf(stderr, "Icon not found\n");
        exit(1);
    }
    SDL_SetColorKey(psIcon, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(psIcon->format, 0, 128, 0));
    SDL_WM_SetIcon(psIcon, NULL);

	// initialize random generator. Pay attention to the different version of srand	
#ifdef _GLIBCPP_HAVE_DRAND48
        srand48((unsigned)time(0));
#else
        srand((unsigned)time(0));
#endif
	
	m_pTitle = IMG_Load(lpszTitleFile);
	if (m_pTitle == 0)
    {
        fprintf(stderr, "Title image not found\n");
        exit(1);
    }

}

////////////////////////////////////////
//       setVideoResolution
/*! Set video resolution
*/
void cEngineApp::setVideoResolution()
{
    if (m_pScreen)
    {
        SDL_FreeSurface(m_pScreen);
    }
	if (m_bFullScreen)
	{
		m_pScreen = SDL_SetVideoMode(m_iScreenW, m_iScreenH, m_iBpp, SDL_FULLSCREEN | SDL_SWSURFACE);
		m_bFullScreen = FALSE;
	}
	else
	{
		m_pScreen = SDL_SetVideoMode(m_iScreenW, m_iScreenH, m_iBpp, SDL_SWSURFACE);
	}
    if ( m_pScreen == NULL )
    {
        fprintf(stderr, "Error setvideomode: %s\n", SDL_GetError());
        exit(1);
        
    }
    screen = m_pScreen;
}


////////////////////////////////////////
//       MainMenu
/*! Run main menu until application end
*/
void cEngineApp::MainMenu()
{
    sdlMenu *menu;
    signed int result,result2;
    BOOL bEnd=FALSE;
    BOOL bRedraw=TRUE;
    int menux,menuy,menuw,menuh;
    string temp;
    
    //CREATE MAIN MENU
    menu=new sdlMenu;
    menu->SetScreen(m_pScreen);
    menu->FilenameBackground = lpszBackGroundFile;
    menuw=445;
    menuh=150;
    menux=(m_pScreen->w-menuw)/2;
    //menuy=m_pScreen->h - menuh - 10;
	menuy= (m_pScreen->h - menuh ) / 2;
    menu->SetArea(menux,menuy,menuw,menuh);
    menu->SetColors(SDL_MapRGBA(m_pScreen->format, 0, 0, 0,0),SDL_MapRGBA(m_pScreen->format, 128, 0, 0,255));
    
    while(bEnd==FALSE)
    {
        //LOAD MENU SONG 
        //theme=FSOUND_Stream_OpenFile("audio/rock.mp3",FSOUND_LOOP_NORMAL,0);
        //PlayAudioStream(theme);
        //AudioFadeIn(0,150);
        
        //SET MAIN MENU ITEMS
        menu->ClearItems();
        menu->AddItems(m_LanString.GetStringId(cLanguages::ID_START));
        menu->AddItems(m_LanString.GetStringId(cLanguages::ID_LANGUAGESEL));
        //menu->AddItems(m_LanString.GetStringId(cLanguages::ID_VIDEOSET));
        //menu->AddItems(m_LanString.GetStringId(cLanguages::ID_MAZZONAME));
        menu->AddItems(m_LanString.GetStringId(cLanguages::ID_SOUNDOPT));
		menu->AddItems(m_LanString.GetStringId(cLanguages::ID_CREDITS));
        // setta il titolo del menu
        menu->SetLabels(m_LanString.GetStringId(cLanguages::ID_MAINMENU),m_LanString.GetStringId(cLanguages::ID_EXIT));
        
        //RUN MENU
        result=menu->Run();
        switch(result)
        {
        case 0:		
            //STARTGAME
            PlayGame();
            //bEnd = TRUE;
            bRedraw=TRUE;
            break;
        case 1:	
            // MENU select a new language
            menu->ClearItems();
            menu->SetLabels(m_LanString.GetStringId(cLanguages::ID_CHOOSELANGUA),m_LanString.GetStringId(cLanguages::ID_BACKMAINMENU));
            menu->AddItems(m_LanString.GetStringId(cLanguages::ID_ITALIANO));
            menu->AddItems(m_LanString.GetStringId(cLanguages::ID_DIALETMN));
            //menu->AddItems(m_LanString.GetStringId(cLanguages::ID_ENGLISH));
            
            result2=menu->Run();	//SHOW LANGUAGE MENU
            // cambiare l'ordine del menu, vuol dire cambiare cambiare l'ordine anche quando 
            // si salva il registro e nella definizione dei linguaggi
            switch(result2)
            {
            case 0:
                // italian
                m_Settings.eLanguageCurrent = cLanguages::LANG_ITA;
                 
                break;
            case 1:
                //dialect mn
                m_Settings.eLanguageCurrent = cLanguages::LANG_DIAL_MN; 
                break;
            case 2:
                // english
                m_Settings.eLanguageCurrent = cLanguages::LANG_ENG; 
                break;
            
                
            }
            m_LanString.SetLang(m_Settings.eLanguageCurrent);
            SDL_WM_SetCaption(m_LanString.GetCStringId(cLanguages::ID_SOLITARIO) ,NULL);
            bRedraw=TRUE;
            break;
			/*
            case 2: // MAZZI DI CARTE
                menuMazziChoose();
                bRedraw=TRUE;
                break;
				*/
            case 2:
                // OPZIONI SUONO MENU
                menu->ClearItems();
                menu->SetLabels(m_LanString.GetStringId(cLanguages::ID_SOUNDOPT),m_LanString.GetStringId(cLanguages::ID_BACKMAINMENU));
                menu->AddItems(m_LanString.GetStringId(cLanguages::ID_ABILITATO));
                menu->AddItems(m_LanString.GetStringId(cLanguages::ID_DISABILITATO));
                result2=menu->Run();
                switch(result2)
                {
                case 0:
                    // sound on
                    m_Settings.bMusicEnabled = TRUE;
                    m_pMusicManager->StartMusic(); 
                    break;
                case 1:
                    //sound off
                    m_Settings.bMusicEnabled = FALSE;
                    m_pMusicManager->StopMusic(); 
                    break;
                }
                break;
			case 3:
				// CREDITS
				credits(m_pScreen, m_pTitle);
				break;

            case 12:	//HIGH SCORE
                hightScoreMenu();
                bRedraw=TRUE;
                break;
            case 122: //VIDEO RESOLUTION MENU
                menu->ClearItems();
                menu->SetLabels(m_LanString.GetStringId(cLanguages::ID_SELECTDISPLAYSETTINGS),m_LanString.GetStringId(cLanguages::ID_BACK));
                menu->AddItems("800x600 x 32bits");
                menu->AddItems("1024x768 x 32bits");
				menu->AddItems("Full screen");
                result2=menu->Run();
                switch(result2)
                {
                case 0:
                    m_iScreenW=800;
                    m_iScreenH=600;
                    m_iBpp=32;
                    break;
                case 1:
                    m_iScreenW=1024;
                    m_iScreenH=768;
                    m_iBpp=32;
                    break;
                case 2:
                    m_iScreenW=1024;
                    m_iScreenH=768;
                    m_iBpp=32;
					m_bFullScreen = TRUE;
                    break;
                }
                setVideoResolution();
                //ADJUST MENU POSITION SINCE SCREEN PROBABLY HAVE CHANGED
                menux=(m_pScreen->w-menuw)/2;
                menu->SetArea(menux,menuy,menuw,menuh);
                menu->SetColors(SDL_MapRGBA(m_pScreen->format, 0, 0, 0,0),SDL_MapRGBA(m_pScreen->format, 128, 0, 0,255));
                //REDRAW MENU
                bRedraw=TRUE;
                break;
            case -1:	
                //EXIT
                    bEnd=TRUE;
                    break;
                default:
                    break;
        }
        
    }
    //DeleteAudioStream(&theme);
    //terminate();
    
    delete menu;
    
}


////////////////////////////////////////
//       WaitKey
/*! Wait key during a menu
*/
int cEngineApp::WaitKey()
{
    SDL_Event event;
    while(1)
    {  
        if (SDL_WaitEvent(&event)==1)
        {
            switch(event.type)
            {  
            case SDL_KEYDOWN:
                return event.key.keysym.scancode;
                break;
            case SDL_MOUSEBUTTONUP:
                return event.button.button;
                break;
            }
        }
    }
}


////////////////////////////////////////
//       hightScoreMenu
/*! Shows the hight score menu
*/
void cEngineApp::hightScoreMenu()
{
    int tx,ty;
    std::string temp;
				
    SDL_FillRect(m_pScreen,&m_pScreen->clip_rect,SDL_MapRGBA(m_pScreen->format, 0, 0, 0,0));
    tx=(m_pScreen->w-400)/2;
    ty=(m_pScreen->h-260)/2;
    
    m_pMainFont->DrawString(m_pScreen,m_LanString.GetStringId(cLanguages::ID_HISCORE),TEXTMIXED,TEXTALIGNCENTER,0,ty-10-SDLFONTSIZE*2,0);
    m_pMainFont->DrawString(m_pScreen,"-------------",TEXTMIXED,TEXTALIGNCENTER,0,ty-10-SDLFONTSIZE,0);
    
    GFX_UTIL::boxRGBA(m_pScreen,tx,ty,tx+400,ty+260,0,0,48,255);
    for (int k=0;k<10;k++)
    {
        temp = STR_UTIL::intToString(m_HScore.HS_Scores[k]);
        m_pMainFont->DrawString(m_pScreen,temp,TEXTMIXED,TEXTALIGNLEFT,tx+5,ty+10+(SDLFONTSIZE+8)*k,0);
        if (m_HScore.HS_Names[k].size()==0)
        {
            temp = m_LanString.GetStringId(cLanguages::ID_ANONIM);
        }
        else 
        {
            temp=m_HScore.HS_Names[k];
        }
        m_pMainFont->DrawString(m_pScreen,temp,TEXTMIXED,TEXTALIGNLEFT,tx+400-15*SDLFONTSIZE,ty+10+(SDLFONTSIZE+8)*k,0);
    }
    m_pMainFont->DrawString(m_pScreen,m_LanString.GetStringId(cLanguages::ID_PUSHBUTTON),TEXTMIXED,TEXTALIGNCENTER,0,ty+260+SDLFONTSIZE,0);
    SDL_Flip(m_pScreen);
    WaitKey();
}


////////////////////////////////////////
//       PlayGame
/*! Play the game
*/
int  cEngineApp::PlayGame()
{
    // card deck
    m_SolitarioGfx.SetDeckType(m_Settings.DeckType);
    m_SolitarioGfx.InitDeck(screen);
    
    
    m_SolitarioGfx.ClearSurface();
    m_SolitarioGfx.Clear();
    m_SolitarioGfx.Initialize(screen);
	//g_CARDWIDTH = 100;
	//g_CARDHEIGHT = 150;

    // crea le regioni solo una sola volta
    // region
    //index 0
    m_SolitarioGfx.CreateRegion(CRD_PILE, CRD_VISIBLE|CRD_3D, 0, 0, CRD_OSYMBOL, 35, 10, 2, 2);
    //index 1-7
    int i;
    for(i=1; i <= 7; i++)
        m_SolitarioGfx.CreateRegion(CRD_FOUNDATION, CRD_VISIBLE|CRD_DODRAG|CRD_DODROP, CRD_DOOPCOLOR|CRD_DOLOWER|CRD_DOLOWERBY1|CRD_DOKING, CRD_DRAGFACEUP, CRD_HSYMBOL, (g_CARDWIDTH * (i - 1)) + (i * 17), g_CARDHEIGHT + 40, 0, 32);
    //index 8
    m_SolitarioGfx.CreateRegion(CRD_RESERVE, CRD_VISIBLE|CRD_FACEUP|CRD_DODRAG|CRD_3D, CRD_DOALL, CRD_DRAGTOP, CRD_NSYMBOL, g_CARDWIDTH + 65, 10, 0, 0);
    //index 9-12
    for(i=4; i <= 7; i++)
        m_SolitarioGfx.CreateRegion(CRD_WASTE, CRD_VISIBLE|CRD_3D|CRD_DODRAG|CRD_DODROP, CRD_DOSINGLE|CRD_DOHIGHER|CRD_DOHIGHERBY1|CRD_DOACE|CRD_DOSUIT, CRD_DRAGTOP, CRD_HSYMBOL, (g_CARDWIDTH * (i - 1)) + (i * 17), 10, 0, 0);
 
    NewGame();
    m_SolitarioGfx.DrawStaticScene();
    
    SDL_Event event;
    int done = 0;
    
    while(done == 0)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
				fade (m_pScreen, m_pScreen, 1, 1) ;
                return 0;
                
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) { done = 1; }
                HandleKeyDownEvent(event);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                HandleMouseDownEvent(event);
                break;
                
            case SDL_MOUSEMOTION:
                HandleMouseMoveEvent(event);
                break;
                
            case SDL_MOUSEBUTTONUP:
                HandleMouseUpEvent(event);
                break;
            }
        }
        if (m_Settings.bMusicEnabled)
        {
            m_pMusicManager->StartMusic(); 
        }
    }
    return 0;
}


////////////////////////////////////////
//       menuMazziChoose
/*! Menu per la scelta dei mazzi di carte 
*/
void cEngineApp::menuMazziChoose()
{
    sdlMenu * menuMazzi=new sdlMenu;
    menuMazzi->SetScreen(m_pScreen);
    menuMazzi->FilenameBackground = lpszBackGroundFile;
    int menuMazziw=445;
    int menuMazzih=500;
    int menuMazzix=(m_pScreen->w - menuMazziw)/2;
    int menuMazziy=m_pScreen->h - menuMazzih - 10;
    menuMazzi->SetArea(menuMazzix, menuMazziy, menuMazziw, menuMazzih);
    menuMazzi->SetColors(SDL_MapRGBA(m_pScreen->format, 0, 0, 0,0),SDL_MapRGBA(m_pScreen->format, 128, 0, 0,255));
    
    menuMazzi->ClearItems();
    menuMazzi->SetLabels(m_LanString.GetStringId(cLanguages::ID_CHOOSEMAZZO),m_LanString.GetStringId(cLanguages::ID_BACKMAINMENU));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_PIACENTINA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_BERGAMO));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_BOLOGNA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_GENOVA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_MILANO));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_NAPOLI));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_PIEMONTE));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_ROMAGNA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_SARDEGNA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_SICILIA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_TOSCANA));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_TRENTO));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_TREVISO));
    menuMazzi->AddItems(m_LanString.GetStringId(cLanguages::ID_TRIESTE));
    
    int result2=menuMazzi->Run();
    
    switch(result2)
    {
    case 0:
        // piacentine
        m_Settings.DeckType.SetType(cTipoDiMazzo::PIACENTINA);  
        break;
    case 1:
        // bergamo
        m_Settings.DeckType.SetType(cTipoDiMazzo::BERGAMO);
        break;
    case 2:
        //bologna
        m_Settings.DeckType.SetType(cTipoDiMazzo::BOLOGNA);
        break;
    case 3:
        //genova
        m_Settings.DeckType.SetType(cTipoDiMazzo::GENOVA);
        break;
    case 4:
        //milano
        m_Settings.DeckType.SetType(cTipoDiMazzo::MILANO);
        break;
    case 5:
        //napoli
        m_Settings.DeckType.SetType(cTipoDiMazzo::NAPOLI);
        break;
    case 6:
        //piemonte
        m_Settings.DeckType.SetType(cTipoDiMazzo::PIEMONTE);
        break;
    case 7:
        //romagna
        m_Settings.DeckType.SetType(cTipoDiMazzo::ROMAGNA);
        break;
    case 8:
        //sardegna
        m_Settings.DeckType.SetType(cTipoDiMazzo::SARDEGNA);
        break;
    case 9:
        //toscana
        m_Settings.DeckType.SetType(cTipoDiMazzo::TOSCANA);
        break;
    case 10:
        //sicilia
        m_Settings.DeckType.SetType(cTipoDiMazzo::SICILIA);
        break;
    case 11:
        //trento
        m_Settings.DeckType.SetType(cTipoDiMazzo::TRENTO);
        break;
    case 12:
        //treviso
        m_Settings.DeckType.SetType(cTipoDiMazzo::TREVISO);
        break;
    case 13:
        //trieste
        m_Settings.DeckType.SetType(cTipoDiMazzo::TRIESTE);
        break;
        
    }
    delete menuMazzi;
}



////////////////////////////////////////
//       setup
/*! 
// \param int argc : 
// \param char * argv[] : 
*/
void  cEngineApp::Setup(int argc, char * argv[])
{
  

  /* Get options from the command line: */
	m_bOverride = FALSE;

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
    {
      /* Display help message: */

   
      printf("Solitario versione 1.5 (c) 2004 Igor Sarzi Sartori\nFai partire il solitario con le seguenti opzioni:\n"
        "--nosound      - to disable sound/music\n"
	"--fullscreen   - to run in fullscreen, if possible (vs. windowed)\n"
    "--size x,y Fa partire il programma ad una risoluzione x,y \n");

      

      exit(0);
    }
    else if (strcmp(argv[i], "--copyright") == 0 ||
	     strcmp(argv[i], "-c") == 0)
    {
      printf(
	"\n\"Solitario\" version 1.5, Copyright (C) 2004 Igor Sarzi Sartori\n"
        "This program is free software; you can redistribute it and/or\n"
        "modify it under the terms of the GNU General Public License\n"
        "as published by the Free Software Foundation.  See COPYING.txt\n"
	"\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
	"\n");

      exit(0);
    }
    else if (strcmp(argv[i], "--usage") == 0 ||
	     strcmp(argv[i], "-u") == 0)
    {
      /* Display (happy) usage: */
	    
      usage(0, argv[0]);
    }
    else if (strcmp(argv[i], "--fullscreen") == 0 ||
	     strcmp(argv[i], "-f") == 0)
    {
      m_bFullScreen = TRUE;
    }
    else if (strcmp(argv[i], "--nosound") == 0 ||
	     strcmp(argv[i], "--quiet") == 0 ||
	     strcmp(argv[i], "-q") == 0)
    {
      m_Settings.bMusicEnabled = false;
	  m_bOverride = TRUE;
    }
    else if (strcmp(argv[i], "--version") == 0 ||
	     strcmp(argv[i], "-v") == 0)
    {
      printf("Solitario versione 1.5\n");
      exit(0);
    }
	else if (strcmp(argv[i], "--size") == 0 ||
	     strcmp(argv[i], "-s") == 0)
    {
		if (i >= argc - 1)
		{
			fprintf(stderr, "%s opzione  richiede 2 argomenti\n", argv[i]);
			usage(1, argv[0]);
		}
		else
		{
			if( parseScreenSize(argv[i+1]) )
			{
			}
			else
			{
				fprintf(stderr, "Parametri non corretti: %s\n", argv[i+1]);
				usage(1, argv[0]);
				exit(1);
			}
			i++;
			
		}
    }
    else
    {
      /* Display 'made' usage: */

      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      usage(1, argv[0]);
    }
  }

 }


////////////////////////////////////////
//       parseScreenSize
/*! 
// \param LPCSTR strOpz : 
*/
BOOL cEngineApp::parseScreenSize(LPCSTR strInput)
{
	char string[2048];
    memset(string, 0, 2048);
	char seps[]   = " ,\t\n";
    char *token;
	VCT_STRINGS vct_String;
	BOOL bRet = FALSE;

	int iNumChar = strlen(strInput);
    strncpy(string, strInput, iNumChar );
    token = strtok( string, seps );
    while( token != NULL )
    {
        vct_String.push_back(token);  
        /* Get next token: */
        token = strtok( NULL, seps );
    }

	int iNumElemntArr = vct_String.size();
    
    if (iNumElemntArr == 2)
    {
        sscanf((LPCSTR)vct_String[0].c_str(), "%d", &m_iScreenW );
		sscanf((LPCSTR)vct_String[1].c_str(), "%d", &m_iScreenH );
		bRet = TRUE;
        
    }
	return bRet;
}


////////////////////////////////////////
//       usage
/*! 
// \param int err : 
// \param char * cmd : 
*/
void cEngineApp::usage(int err, char * cmd)
{
  FILE * f;

  if (err == 0)
    f = stdout;
  else
    f = stderr;


  fprintf(f,
   "\nUsage: %s {--help | --usage | --copyright}\n"
   "       %s [--fullscreen] [--nosound] [--size x,y] "
    "\n", cmd, cmd);

  exit (err);
}

