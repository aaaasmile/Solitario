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


/*
	filename:		CGame.h
	author:			Rico Roberto Zuñiga
	last modified:	9/12/03

	description:	Interface for game data strucuture.
*/

#ifndef CGAME_H
#define CGAME_H

#include <vector>

#include "CCardRegion.h"
#include "GameSettings.h"
#include "win_type_global.h"

typedef std::vector<CCardRegion>::iterator rVI;

struct DRAGCARD
{
	int x;
	int y;
	int width;
	int height;
};

const unsigned int CRD_NSYMBOL	=	0;
const unsigned int CRD_XSYMBOL	=	1;
const unsigned int CRD_OSYMBOL	=	2;
const unsigned int CRD_HSYMBOL	=	3;

class CGame : public std::vector<CCardRegion>
{
	enum
	{
		NUM_CARDS_ONDECK = 40,
		NUM_SYMBOLS = 4
	};
public:

	CGame();
	~CGame();

	void Initialize(SDL_Surface *s);
 	

	void Clear()	{ this->clear(); }
	int  Size()		{ return this->size(); }

//----------------------------------------------------------------------------------------
//Defined in CGame.cpp

	void CreateRegion (int id, unsigned int attribs, unsigned int amode, int dmode, int symbol, int x, int y, int xoffset, int yoffset);
	bool DeleteRegion(CCardRegion *pRegion);

   	void EmptyStacks();
	void InitAllCoords();

//----------------------------------------------------------------------------------------
//Drag and Drop

	CCardRegion *OnMouseDown(int x, int y);

	bool InitDrag(int x, int y);
	bool InitDrag(CCardStack *CargoStack, int x, int y);

	void DoDrag(int x, int y);

	void DoDrop();
	void DoDrop(CCardRegion *DestRegion);

	CCardRegion *FindDropRegion(int Id, CCard card);
	CCardRegion *FindDropRegion(int Id, CCardStack stack);

	void ZoomCard(int &sx, int &sy, int &dx, int &dy, int width, int height, SDL_Surface *bg, SDL_Surface *fg);

	void DrawStaticScene();
	void DrawBackground(BOOL bIsInit);

	CCardRegion *GetBestStack(int x, int y, int width, int height, CCardStack *stack);
//----------------------------------------------------------------------------------------
// card graphics
    int InitDeck(SDL_Surface *s);
    int LoadCardPac();

    int DrawCard(int x, int y, int nCdIndex);
    int DrawCard(int x, int y, int nCdIndex, SDL_Surface *s);
    int DrawCard(VI vi, SDL_Surface *s);

    int DrawCardBack(int x, int y);
    int DrawCardBack(int x, int y, SDL_Surface *s);

    int DrawSymbol(int x, int y, int nSymbol);
    int DrawSymbol(int x, int y, int nSymbol, SDL_Surface *s);


    int AnimateCards(); //might take seconds as parameter
    void SetDeckType(cTipoDiMazzo &Val){m_DeckType.Clone(Val);}
    void ClearSurface();

	void LoadCustomDeck();
	void LoadSymbols();

private:

	CCardStack DragStack;
	CCardRegion *SourceRegion;

	SDL_Surface *screen;
	SDL_Surface *background;
	SDL_Surface *dragface;
	SDL_Surface *scene_background;

	DRAGCARD dcard;

	int oldx;
	int oldy;

    SDL_Surface *deck;
    SDL_Surface *scr;
    SDL_Surface *symbols;

    SDL_Rect SrcBack;
    SDL_Rect SrcCard;

    //! deck type
    cTipoDiMazzo    m_DeckType;
	//! deck cards
	SDL_Surface*    m_pCardsSurf[NUM_CARDS_ONDECK];
	//! symbols
	SDL_Surface*    m_pSymbol[NUM_SYMBOLS];

};

#endif //CGAME_H
