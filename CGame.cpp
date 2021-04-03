////////////////////////////////////////////////////////////////////////////////
// File         : cGame.cpp 
// Project      : 
// Subsystem    : 
// Component    : 
// Author       : Igor Sarzi Sartori (c) 2004 adapted from Rico Roberto Zuñiga
// Description  : implementation of the cMusicManager class.
////////////////////////////////////////////////////////////////////////////////
// History      : 8.05.2004   Igor Sarzi Sartori    Created

/**
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

#include <stdio.h>
#include <SDL_image.h>
#include <SDL_endian.h>

#include "CGame.h"
#include "win_type_global.h"
#include "fading.h"
#include "gfx_util.h"

typedef std::vector<std::string> VCT_STRINGS;

// card graphics
unsigned int g_CARDWIDTH = 0	;
unsigned int g_CARDHEIGHT = 0	;
unsigned int g_SYMBOLWIDTH = 84;
unsigned int g_SYMBOLHEIGHT	=	144;


int const CARDBACKLOC = 40 * g_CARDWIDTH; 						//modify to allow custom card backs

#define FPS 30
#define FRAMETICKS (1000/FPS)
#define THINKINGS_PER_TICK 1

static const char * lpszBackGGameFile = DATA_PREFIX"im001537.jpg";
static const char * lpszMazziDir = DATA_PREFIX"mazzo/";
static const char * lpszDataDir = DATA_PREFIX;

// constructor
CGame::CGame()
{
	background = 0;
    scene_background = 0;
	for (int i = 0; i < NUM_CARDS_ONDECK; i++)
	{
		m_pCardsSurf[i] = 0;
	}
}


////////////////////////////////////////
//       ~CGame
/*! Destructor
*/
CGame::~CGame() 
{ 
   ClearSurface();    
}


////////////////////////////////////////
//       ClearSurface
/*! clear stuff
*/
void CGame::ClearSurface()
{
    if (background)
	    SDL_FreeSurface(background); 
    if (scene_background)
	    SDL_FreeSurface(scene_background); 
}

void CGame::CreateRegion(int id, unsigned int attribs, unsigned int amode, int dmode, int symbol, int x, int y, int xoffset, int yoffset)
{
    CCardRegion *cr = new CCardRegion(id, attribs, amode, dmode, symbol, x, y, xoffset, yoffset, this);
	this->push_back(*cr);
}

bool CGame::DeleteRegion(CCardRegion *pRegion)
{
	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		if(&(*vi) == pRegion)
		{
			this->erase(vi);
			return true;
		}
	}

	return false;
}

void CGame::EmptyStacks()
{
    for(rVI vi = this->begin(); vi != this->end(); ++vi)
        vi->Clear();
}

void CGame::InitAllCoords()
{
	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		vi->InitCardCoords();
		vi->InitCardFaces();
	}
}

//------------------------------------------------------------------------------------------------
//Drag and drop

CCardRegion* CGame::OnMouseDown(int x, int y)
{
	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		if(vi->PtInStack(x, y) )
		{
			SourceRegion = &(*vi);
			return SourceRegion;
		}
	}
	return NULL;
}

bool CGame::InitDrag(int x, int y)
{
	return InitDrag(NULL, x, y);
}

// optimization needed
bool CGame::InitDrag(CCardStack *CargoStack, int x, int y)
{
	if(CargoStack == NULL)
	{
		if(SourceRegion->Empty())
			return false;

		int idx = SourceRegion->GetClickedCard(x, y);
		if(idx == -1)
			return false; // no card found

		switch(SourceRegion->GetDragMode())
		{
			case CRD_DRAGCARDS:
				DragStack.Push(SourceRegion->RemoveCard(idx));
				break;

			case CRD_DRAGTOP:
				if( SourceRegion->Size() - 1 == idx )
					DragStack.Push(SourceRegion->Pop());
				else
					return false;
				break;

			case CRD_DRAGFACEUP:
				if(SourceRegion->CardFaceUp(idx))
					DragStack.Push( SourceRegion->Pop( SourceRegion->Size() - idx ) );
 				else
 					return false;
 				break;

			default: // CRD_DRAGSTACKS
				DragStack.Push( SourceRegion->Pop( SourceRegion->Size() - idx ) );
		}
	}
	else
		DragStack.Push(*CargoStack);

	SourceRegion->InitCardCoords();

	DrawBackground(FALSE);
	SDL_BlitSurface(screen, NULL, background, NULL);

	CCardRegion DragRegion(0, SourceRegion->GetAttributes()|CRD_FACEUP, 0, 0, 0, 0, 0, SourceRegion->GetxOffset(), SourceRegion->GetyOffset(), this);
	DragRegion.Push(DragStack);

	dcard.x = DragStack[0].x;
	dcard.y = DragStack[0].y;
	dcard.width = DragRegion.GetStackWidth();
	dcard.height = DragRegion.GetStackHeight();

	dragface = SDL_CreateRGBSurface(SDL_SWSURFACE, dcard.width, dcard.height, 32, 0, 0, 0, 0);
	SDL_FillRect(dragface, NULL, SDL_MapRGB(dragface->format, 0, 255, 0));
	SDL_SetColorKey(dragface, SDL_SRCCOLORKEY, SDL_MapRGB(dragface->format, 0, 255, 0));

	DragRegion.DrawCardStack(screen);
	DragRegion.InitCardCoords();
	DragRegion.DrawCardStack(dragface);

	oldx = x;
	oldy = y;

	SDL_Flip(screen);
	return true;
}

// optimization needed
void CGame::DoDrag(int x, int y)
{
	SDL_Rect rcs;
	SDL_Rect rcd;

	rcs.x = dcard.x - 2;
	rcs.y = dcard.y - 2;
	rcs.w = dcard.width + 4;
 	rcs.h = dcard.height + 4;

	rcd.x = dcard.x - 2;
 	rcd.y = dcard.y - 2;

	if(dcard.x < 0) rcs.x = rcd.x = 0;
	if(dcard.y < 0) rcs.y = rcd.y = 0;

	if(x < oldx) dcard.x -= oldx - x; else dcard.x += x - oldx;
	if(y < oldy) dcard.y -= oldy - y; else dcard.y += y - oldy;

	oldx = x;
	oldy = y;

	SDL_Rect dest;
	dest.x = dcard.x;
	dest.y = dcard.y;

	SDL_BlitSurface(background, &rcs, screen, &rcd);
	SDL_BlitSurface(dragface, NULL, screen, &dest);

	SDL_Flip(screen);
}

void CGame::DoDrop()
{
	DoDrop(NULL);
}
// optimization needed
void CGame::DoDrop(CCardRegion *DestRegion)
{
	CCardStack *DestStack;
	CCardRegion *BestRegion;

	if(DestRegion != NULL) 
		BestRegion = DestRegion;
	else 
		BestRegion = this->GetBestStack(dcard.x, dcard.y, g_CARDWIDTH, g_CARDHEIGHT, &DragStack);
	if(BestRegion == NULL) 
		BestRegion = SourceRegion;

	DestStack = BestRegion->GetCardStack();
	DestStack->Push(DragStack);
	BestRegion->InitCardCoords();

	VI vi;
	switch(SourceRegion->GetDragMode())
	{
		case 2:
		case 3:
			vi = DestStack->end() - 1;
			break;
		default: // 1 and 4
			vi = DestStack->end() - DragStack.Size();
	}

	DragStack.Clear();

	//when no movement
	if(dcard.x == vi->x && dcard.y == vi->y)
		return;

	ZoomCard(dcard.x, dcard.y, vi->x, vi->y, dcard.width, dcard.height, background, dragface);
}
CCardRegion* CGame::FindDropRegion(int Id, CCard card)
{
	CCardStack stack;
	stack.Push(card);
	return FindDropRegion(Id, stack);
}

CCardRegion* CGame::FindDropRegion(int Id, CCardStack stack)
{
	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		if((vi->Id == Id) && vi->CanDrop(&stack))
			return &(*vi);
	}
	return NULL;
}

void CalcPt(int x0, int y0, int x1, int y1, float t, int &X, int &Y)
{
	X = int(x0 + t * (x1 - x0) + .5);
	Y = int(y0 + t * (y1 - y0) + .5);
}

void CGame::ZoomCard(int &sx, int &sy, int &dx, int &dy, int w, int h, SDL_Surface *bg, SDL_Surface *fg)
{
	SDL_Rect rcs;
	SDL_Rect rcd;
	SDL_Rect dest;

	int px, py;
	float precision = 0.1f;

	for(float i = 0.0; i <= 1.0; i += precision)
	{
		SDL_PumpEvents();
		CalcPt(sx, sy, dx, dy, i, px, py);

		rcs.x = sx - 2;
		rcs.y = sy - 2;
		rcs.w = w + 4;
 		rcs.h = h + 4;

		rcd.x = sx - 2;
	 	rcd.y = sy - 2;

		if(sx < 0) rcs.x = rcd.x = 0;
		if(sy < 0) rcs.y = rcd.y = 0;

		sx = dest.x = px;
		sy = dest.y = py;

		SDL_BlitSurface(bg, &rcs, screen, &rcd);	//background
		SDL_BlitSurface(fg, NULL, screen, &dest);	//foreground

		SDL_Flip(screen);
	}
	DrawStaticScene();
	SDL_FreeSurface(dragface);
}

void CGame::DrawStaticScene()
{
	
	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		SDL_PumpEvents();
		vi->DrawCardStack();
	}

	SDL_Flip(screen);
}


////////////////////////////////////////
//       DrawBackground
/*! Draw background of the game
*/
void CGame::DrawBackground(BOOL bIsInit)
{
	if (scene_background)
    {
        if (!bIsInit)
        {
            SDL_BlitSurface(scene_background, NULL, screen, NULL);
        }
        else
        {
            fade (screen, scene_background, 2, 0) ;
        }
    }

	for(rVI vi = this->begin(); vi != this->end(); ++vi)
		vi->DrawCardStack();
}

CCardRegion *CGame::GetBestStack(int x, int y, int w, int h, CCardStack *stack)
{
	int maxoverlap =  0;
	int percent = 0;
	CCardRegion *best = 0;

	for(rVI vi = this->begin(); vi != this->end(); ++vi)
	{
		SDL_PumpEvents();
		if(vi->CanDrop(stack))
			percent = vi->GetOverlapRatio(x, y, w, h);

		if(percent > maxoverlap)
		{
			maxoverlap = percent;
			best = &(*vi);
		}

	}
	return best;
}


////////////////////////////////////////
//       Initialize
/*! initialize the game
// \param SDL_Surface *s : screen surface 
*/
void CGame::Initialize(SDL_Surface *s)
{
  	screen = s;
	background = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
	// background
    SDL_Surface *Temp;
    SDL_RWops *srcBack = SDL_RWFromFile(lpszBackGGameFile, "rb");
    Temp = IMG_LoadJPG_RW(srcBack);
    scene_background = SDL_DisplayFormat(Temp);
    SDL_FreeSurface(Temp);
	
	DrawBackground(TRUE);
}




////////////////////////////////////////
//       InitDeck
/*! Inizializza il mazzo
// \param SDL_Surface *s : 
*/
int CGame::InitDeck(SDL_Surface *s)								
{
    // load deck from pac file
	LoadCustomDeck();
	scr = s;

	LoadSymbols();
    
	return 0;
}


////////////////////////////////////////
//       DrawCard
/*! 
// \param int x : 
// \param int y : 
// \param int nCdIndex : 
*/
int CGame::DrawCard(int x, int y, int nCdIndex)
{
	return DrawCard(x, y, nCdIndex, NULL);
    
}


////////////////////////////////////////
//       DrawCard
/*! Disegna una carta da briscola. Il mazzo è composto dai segni disposti in modo verticale.
// L'ordine dei segni è: bastoni, coppe, denari, spade.
// \param int x : x coordinata destination
// \param int y : y coordinate
// \param int nCdIndex : card index
// \param SDL_Surface *s : screen surface
*/
int CGame::DrawCard(int x, int y, int nCdIndex, SDL_Surface *s)
{
	if(s == NULL)
  		s = scr;

	if(nCdIndex < 0) 
        nCdIndex = 0;
	if(nCdIndex > NUM_CARDS) 
        nCdIndex = NUM_CARDS - 1;

	int iSegnoIx = nCdIndex / 10;
	int iCartaIx = nCdIndex % 10;

	//SrcCard.x = iSegnoIx * g_CARDWIDTH;
    //SrcCard.y = iCartaIx * g_CARDHEIGHT;
	SrcCard.x = 0;
    SrcCard.y = 0;
    SrcCard.w = m_pCardsSurf[nCdIndex]->clip_rect.w ;
	SrcCard.h = m_pCardsSurf[nCdIndex]->clip_rect.h ;


	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	return SDL_BlitSurface(m_pCardsSurf[nCdIndex], &SrcCard, s, &dest);
}


////////////////////////////////////////
//       DrawCard
/*! 
// \param VI vi : 
// \param SDL_Surface *s : 
*/
int CGame::DrawCard(VI vi, SDL_Surface *s)
{
	if(s == NULL)
  		s = scr;

    int nCdIndex = vi->Idx;

	if(nCdIndex < 0) 
        nCdIndex = 0;
	if(nCdIndex > NUM_CARDS) 
        nCdIndex = NUM_CARDS - 1;

	
	//SrcCard.x = iSegnoIx * g_CARDWIDTH;
    //SrcCard.y = iCartaIx * g_CARDHEIGHT;
    SrcCard.x = 0;
    SrcCard.y = 0;
    SrcCard.w = m_pCardsSurf[nCdIndex]->clip_rect.w ;
	SrcCard.h = m_pCardsSurf[nCdIndex]->clip_rect.h ;

	SDL_Rect dest;
	dest.x = vi->x;
	dest.y = vi->y;

    TRACE("Draw card ix = %d, segno = %d, valore %d \n", vi->Idx, vi->Suit(), vi->Rank());
    
	return SDL_BlitSurface(m_pCardsSurf[nCdIndex], &SrcCard, s, &dest);
}


////////////////////////////////////////
//       DrawCardBack
/*! Disegna il retro della carta
// \param int x : 
// \param int y : 
*/
int CGame::DrawCardBack(int x, int y)
{
	return DrawCardBack(x, y, NULL);
}


////////////////////////////////////////
//       DrawCardBack
/*! Disegna il dorso della carta
// \param int x : 
// \param int y : 
// \param SDL_Surface *s : 
*/
int CGame::DrawCardBack(int x, int y, SDL_Surface *s)
{
	if(s == NULL)
  		s = scr;

	SrcCard.x = 0;
    SrcCard.y = 0;
    SrcCard.w = m_pSymbol[0]->clip_rect.w ;
	SrcCard.h = m_pSymbol[0]->clip_rect.h ;

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	return SDL_BlitSurface(m_pSymbol[0], &SrcCard, s, &dest);
}


////////////////////////////////////////
//       DrawSymbol
/*! 
Symbols:
		0:	Blank, no symbol
		1:	X symbol
		2:	O symbol
		3:	Holder symbol
// \param int x : 
// \param int y : 
// \param int nSymbol : 
*/
int CGame::DrawSymbol(int x, int y, int nSymbol)
{
	return DrawSymbol(x, y, nSymbol, NULL);
}


////////////////////////////////////////
//       DrawSymbol
/*! Disegna uno dei simboli dalla tabella dei simboli. 4 simboli numerati da 0 a 3
// \param int x : 
// \param int y : 
// \param int nSymbol : 
// \param SDL_Surface *s : 
*/
int CGame::DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s)
{
	if(nSymbol < 1) 
		return 0;
	if(nSymbol > 3) 
		nSymbol = 3;

	if(s == NULL)
  		s = scr;

	SrcCard.x = 0;
    SrcCard.y = 0;
    SrcCard.w = m_pSymbol[nSymbol]->clip_rect.w ;
	SrcCard.h = m_pSymbol[nSymbol]->clip_rect.h ;

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	return SDL_BlitSurface(m_pSymbol[nSymbol], &SrcCard, s, &dest);
}



////////////////////////////////////////
//       AnimateCards
/*! Mostra una piccola animazione delle carte
*/
int CGame::AnimateCards()
{
	srand((unsigned)time(NULL));

	int rot;
	int id;
	int x;
	unsigned int y;
	int xspeed;
	int yspeed;

	int		GRAVITY = 1;
	unsigned int		MAXY = scr->h;
	float	BOUNCE = 0.8f;

	do
	{
		rot = rand() % 2;
		id = rand() % 51;
		x  = rand() % scr->w;
		y  = rand() % scr->h/2;

		if(rot)
			xspeed = -4;
		else
			xspeed =  4;

		yspeed = 0;

		do //while card is within the screen
		{
			SDL_PumpEvents();
			if(SDL_GetMouseState(NULL, NULL))
				return -1; // stop the animation

			yspeed = yspeed + GRAVITY;
			x += xspeed;
			y += yspeed;

			if(y + g_CARDHEIGHT > MAXY)
			{
				y = MAXY - g_CARDHEIGHT;
				yspeed = int(-yspeed * BOUNCE);
			}

			DrawCard(x, y, id, scr);
			SDL_Flip(scr);
		}
		//73 here is CARDWIDTH, but when using CARDWIDTH, it doesn't work
		while((x + 73 > 0) && (x < scr->w));
//		while((x + CARDWIDTH > 0) && (x < scr->w));
	}
	while(1); //or while within specified time

	return 0;
}


////////////////////////////////////////
//       LoadCustomDeck
/*! Load card from custom deck
*/
void CGame::LoadCustomDeck()
{
	SDL_Surface *Temp;

	std::string strTmp;
	std::string strSuffix;
	CHAR buff[128];

	
	for( int k = 0; k < 4; k++)
	{
		std::string strFileName = lpszMazziDir;
		switch (k)
		{
			case 0:
				strSuffix = "bastoni";
				strFileName += "bastoni/";
				break;
			case 1:
				strSuffix = "coppe";
				strFileName += "coppe/";
				break;
			case 2:
				strSuffix = "denari";
				strFileName += "denari/";
				break;
			case 3:
				strSuffix = "spade";
				strFileName += "spade/";
				break;
		}
		for (int i = 0; i < 10; i++)
		{
			sprintf(buff,"%02d_%s.jpg", i+1, strSuffix.c_str());
			strTmp = buff;
			std::string  strComplete = strFileName + strTmp;
			SDL_RWops *srcBack = SDL_RWFromFile(strComplete.c_str(), "rb");
			
			if (srcBack)
			{
				Temp = IMG_LoadJPG_RW(srcBack);
				if (Temp)
				{
					m_pCardsSurf[i+ k*10] = SDL_DisplayFormat(Temp);
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	SDL_FreeSurface(Temp);

	if (m_pCardsSurf[0])
	{
		g_CARDWIDTH = m_pCardsSurf[0]->clip_rect.w;
 		g_CARDHEIGHT = m_pCardsSurf[0]->clip_rect.h;
	}

}


////////////////////////////////////////
//       LoadSymbols
/*! Load all symbols from jpg files
*/
void CGame::LoadSymbols()
{
// symbols
	VCT_STRINGS vct_Strings;
	vct_Strings.push_back("dorso.jpg");
	vct_Strings.push_back("fine_1.jpg");
	vct_Strings.push_back("fine_2.jpg");
	vct_Strings.push_back("fine_3.jpg");
	ASSERT(NUM_SYMBOLS == vct_Strings.size());

	SDL_Surface *Temp = 0;
	for (int i = 0; i < NUM_SYMBOLS; i++)
	{

		std::string strDir = lpszMazziDir;
		std::string strFileSymbName ;
		strFileSymbName = lpszMazziDir + vct_Strings[i];
		SDL_RWops *srcBack = SDL_RWFromFile(strFileSymbName.c_str(), "rb");
		if (srcBack)
		{
			Temp = IMG_LoadJPG_RW(srcBack);
			if (Temp)
			{
				m_pSymbol[i] = SDL_DisplayFormat(Temp);
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			ASSERT(0);
		}
	}
	SDL_FreeSurface(Temp);
    //SDL_SetColorKey(symbols, SDL_SRCCOLORKEY, SDL_MapRGB(symbols->format, 242, 30, 206));	
}

////////////////////////////////////////
//       LoadCardPac
/*! Carica il mazzo delle carte dal file in formato pac
*/
int CGame::LoadCardPac()
{
    Uint32          timetag;
    char            describtion[100];
    Uint8           num_anims;
    Uint16          w, h;
    Uint16          frames;
    Uint16        *delays;
    
    std::string strFileName = lpszMazziDir;
    strFileName += m_DeckType.GetResFileName();

    SDL_RWops *src = SDL_RWFromFile(strFileName.c_str(), "rb");
    SDL_RWread(src, describtion, 100, 1);
    timetag = SDL_ReadLE32(src);
    SDL_RWread(src, &num_anims, 1, 1);
    w = SDL_ReadLE16(src); // witdh of the picture (pac of 4 cards)
    h = SDL_ReadLE16(src); // height of the picture (pac of 10 rows of cards)
    frames = SDL_ReadLE16(src);

    delays = (Uint16*)malloc(sizeof(Uint16)*frames);
    if (!delays) 
        return 2;

    for (int i=0; i<frames; i++) 
    {
        // file format stores delays in 1/100th of second
        // we will convert them to game ticks
        delays[i] = THINKINGS_PER_TICK*((10*SDL_ReadLE16(src))/FRAMETICKS);  
    }

    SDL_Surface *s;
    SDL_Surface *temp;
    temp = IMG_LoadPNG_RW(src); 
    if (!temp) 
        return 0;

    if(SDL_WasInit(SDL_INIT_VIDEO)!=0)     
    {
        // converte l'immagine al formato video scelto
        s = SDL_DisplayFormat(temp);      // we are in game
        SDL_FreeSurface(temp);
        // setta il pixel trasparente
        SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(s->format, 0, 128, 0));
    }
    else
        s = temp;                         // we are dedicated windows traybar server

    deck = s;
	g_CARDWIDTH = w/4;
    g_CARDHEIGHT = h/10;

	return 0;
}
